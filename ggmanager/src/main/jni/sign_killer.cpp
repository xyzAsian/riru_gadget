//
// Created by 1 on 2020/12/8.
//

#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include "sign_killer.h"
#include "cmd.h"
#include "log.h"
#include <threads.h>
#include <string.h>
#include "kill.h"
#include <jni.h>
#include <sys/prctl.h>
#include <wait.h>

int fd[2], sync_c[2], sync_p[2];
pid_t child_pid;

pthread_mutex_t mutex;

void set_checkbutton(JNIEnv* env ,bool state) {
    jclass nativelib = env->FindClass("com/gadget/manager/utils/NativeLib");
    jmethodID  changeSignKillerState = env->GetStaticMethodID(nativelib, "changeSignKillerState","(Z)V");
    env->CallStaticVoidMethod(nativelib, changeSignKillerState, (jboolean)state);
}

struct thdata {
    int fd_r;
    int fd_w;
    pid_t pid;
} ;

void *keep_pipe_connect_parent(void *ptr) {
    thdata *data = (thdata *)ptr;
    int fd_w = data->fd_w;
    int fd_r = data->fd_r;
    pid_t pid = data->pid;
//    free(ptr) ;

    LOGV("[%s] fdr[%d] fdw[%d] childpid[%d], tid[%d]", __FUNCTION__,fd_r, fd_w, pid, gettid());

    int32_t buf = 1;
    write(fd_w, &buf, sizeof(int32_t));

    buf = 0;
    do {
        int ret = read(fd_r, &buf, sizeof(int32_t));
        if(ret != -1) {
            if (buf == 1) {
                LOGV("read child return 1, success");
                //子进程返回成功 success
//                set_checkbutton(env, true);
            } else if (buf == 2) {
                //子进程返回退出
//                set_checkbutton(env, false);
                LOGW("[%s] before kill pid[%d], tid[%d], ret=[%d]", __FUNCTION__, pid, gettid(), ret);
                break;
            }
        }
    } while(1) ;

    close(fd_r);
    close(fd_w);

    return NULL;
}

void *wait_for_child(void *ptr) {
    thdata *data = (thdata *)ptr;
    int fd_w = data->fd_w;
    int fd_r = data->fd_r;
    pid_t pid = data->pid;
    LOGW("wait_for_child:waitpid start %d",pid);
    waitpid(pid,NULL,0);
    LOGW("wait_for_child:waitpid end %d",pid);
    close(fd_r);
    close(fd_w);
}


void *start_frida_inject_thread(void *ptr) {

    sleep(2);
    thdata *data = (thdata *)ptr;
    int fd_r = data->fd_r;
    int fd_w = data->fd_w;
//    free(ptr);
    LOGV("[%s] fd_r[%d] fd_w[%d]", __FUNCTION__, fd_r, fd_w);

    char result[1024];
#if defined(__x86_64__) || defined(__aarch64__)
    //exec_cmd("su -c \"/system/lib64/frida-inject-14.1.2 -n system_server -s /system/lib64/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
    exec_cmd("su -c \"/data/local/tmp/frida-inject-14.1.2 -n system_server -s /data/local/tmp/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
#else
    //exec_cmd("su -c \"/system/lib/frida-inject-14.1.2 -n system_server -s /system/lib64/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
    exec_cmd("su -c \"/data/local/tmp/frida-inject-14.1.2 -n system_server -s /data/local/tmp/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
#endif

    LOGE("[%s] exec frida_inject error",__FUNCTION__ );
    int32_t buf;
    //如果上面命令执行失败，则会走到这句
    buf = 2;// 1 表示子进程退出前，返回给主进程状态
    write(fd_w, &buf, sizeof(int32_t));
}

void *keep_pipe_connect_child(void *ptr) {
    thdata *data = (thdata *)ptr;
    int fd_r = data->fd_r;
    int fd_w = data->fd_w;
    pid_t parent_pid = data->pid;
//    free(ptr);

    pid_t pid = getpid();
    LOGV("[%s] fdr[%d] fdw[%d] parent_pid[%d] pid[%d], tid[%d]", __FUNCTION__, fd_r, fd_w , parent_pid, pid, gettid());

    int ret;
    int32_t buf;
    while (1){
        buf = 0;
        ret = read(fd_r, &buf, sizeof(buf));
        if(ret != -1) {
            if (buf == 1) {
                LOGV("[%s] read buf=1 from parent. start sign killer server.", __FUNCTION__ );
                int try_count = 5;
                pthread_t pthread;
                while (pthread_create(&pthread, NULL, start_frida_inject_thread, data) && try_count > 0) {
                    sleep(1);
                    --try_count;
                }
            } else if (buf == 2) {
                //主进程发送退出命令
                LOGV("[%s] read buf=2 from parent. stop sign killer server.", __FUNCTION__ );
                buf = 1;// 1 表示子进程退出前，返回给主进程状态
                write(fd_w, &buf, sizeof(int32_t));
                break;
            }
        }
    }

    close(fd_w);
    close(fd_r);

    mykillex(0x95270000, pid);
    exit(0);
}

void start_kill_signcheck(JNIEnv* env) {

    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutex, &mutexattr);

    pipe(fd);
    pipe(sync_c);
    pipe(sync_p);

    pid_t parent_pid = getpid();
    pid_t pid = fork();
    if (pid > 0) {// parent

        child_pid = pid;
        close(fd[1]);
        close(sync_c[0]);
        close(sync_p[1]);

        thdata* data = (thdata*)malloc(sizeof(thdata));
        memset(data, 0, sizeof(thdata));
        data->fd_w = sync_c[1];
        data->fd_r = sync_p[0];
        data->pid = pid;

        LOGV("thdata fdr:%d, fdw:%d, childpid:%d",data->fd_r, data->fd_w, data->pid);
        int try_count = 5;
        pthread_t threadid;
        while (pthread_create(&threadid,NULL, keep_pipe_connect_parent, data) && try_count > 0) {
            LOGW("waring [%s] start keep_pipe_connect_parent thread error , sleep 1 second retry.", __FUNCTION__ );
            sleep(1);
            --try_count;
        }

        try_count = 5;
        pthread_t threadid2;
        while (pthread_create(&threadid2,NULL, wait_for_child, data) && try_count > 0) {
            LOGW("waring [%s] start keep_pipe_connect_parent thread error , sleep 1 second retry.", __FUNCTION__ );
            sleep(1);
            --try_count;
        }

    } else {//child

        prctl(PR_SET_NAME,"xyzggmanager_ch",NULL,NULL,NULL);

        close(fd[0]);
        close(sync_c[1]);
        close(sync_p[0]);
        thdata* data = (thdata*)malloc(sizeof(thdata));
        memset(data, 0, sizeof(thdata));
        data->fd_r = sync_c[0];
        data->fd_w = sync_p[1];
        data->pid = parent_pid;

        LOGV("thdata fdr:%d, fdw:%d, parent_pid:%d",data->fd_r, data->fd_w, data->pid);

        //子进程开启通信线程
        int try_count = 5;
        pthread_t threadid;
        while (pthread_create(&threadid,NULL, keep_pipe_connect_child, data) && try_count > 0) {
            LOGW("waring [%s] start keep_pipe_connect_child thread error , sleep 1 second retry.", __FUNCTION__ );
            sleep(1);
            --try_count;
        }

        //子进程开启frida hook进程
        try_count = 5;
        pthread_t pthread2;
        while (pthread_create(&pthread2, NULL, start_frida_inject_thread, data) && try_count > 0) {
            LOGW("waring [%s] start start_frida_inject_thread thread error , sleep 1 second retry.", __FUNCTION__ );
            sleep(1);
            --try_count;
        }
    }
}

void stop_kill_signcheck() {
    int32_t buf;
    buf = 2;
    write(sync_c[1], &buf, sizeof(int32_t));

    if(child_pid != 0) {
//        mykillex2(0x95270000, child_pid);
    }
}

int is_child_alive() {
    char result[20];
    exec_cmd("su -c \"ps | grep xyzggmanager_ch | wc -l\"", result, sizeof(result), false);
    return strncmp(result, "1", strlen("1"));
}