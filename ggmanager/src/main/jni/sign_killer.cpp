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

#include "sign_killer_signal.h"

#define WRITE_PIPE 1
#define READ_PIPE 0

int sync_c[2], sync_p[2];
int32_t sign_kill_server_status = JNI_FALSE;


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

void *wait_for_child(void *ptr) {
    thdata *data = (thdata *)ptr;
    int fd_w = data->fd_w;
    int fd_r = data->fd_r;
    pid_t pid = data->pid;
    LOGW("wait_for_child:waitpid start %d",pid);
    waitpid(pid,NULL,0);
    LOGW("wait_for_child:waitpid end %d",pid);
    if(fd_r != 0) close(fd_r);
    if(fd_w != 0) close(fd_w);
    free(ptr);
}


void *start_frida_inject_thread(void *ptr) {
    int fd_r;
    int fd_w;
    sleep(2);
    thdata *data = (thdata *)ptr;
    fd_r = data->fd_r;
    fd_w = data->fd_w;
    LOGV("[%s] fd_r[%d] fd_w[%d] tid[%d]", __FUNCTION__, fd_r, fd_w, gettid());

    sign_kill_server_status = JNI_TRUE;

    char result[1024];
#if defined(__x86_64__) || defined(__aarch64__)
    exec_cmd("su -c \"chmod 777 system/lib64/frida-inject-14.1.2;/system/lib64/frida-inject-14.1.2 -n system_server -s /system/lib64/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
//    exec_cmd("su -c \"/data/local/tmp/frida-inject-14.1.2 -n system_server -s /data/local/tmp/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
#else
    exec_cmd("su -c \"chmod 777 system/lib/frida-inject-14.1.2;/system/lib/frida-inject-14.1.2 -n system_server -s /system/lib64/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
//    exec_cmd("su -c \"/data/local/tmp/frida-inject-14.1.2 -n system_server -s /data/local/tmp/libxyzggAndroidSignature.js\"", result, sizeof(result),true);
#endif

    LOGE("[%s] exec frida_inject error",__FUNCTION__ );
    sign_kill_server_status = JNI_FALSE;
}

void *keep_pipe_connect_child(void *ptr) {
    thdata *data = (thdata *)ptr;
    int fd_r = data->fd_r;
    int fd_w = data->fd_w;
    pid_t parent_pid = data->pid;

    pid_t pid = getpid();
    LOGV("[%s] fdr[%d] fdw[%d] parent_pid[%d] pid[%d], tid[%d]", __FUNCTION__, fd_r, fd_w , parent_pid, pid, gettid());

    int ret;
    int32_t buf;
    pthread_t pthread = 0l;
    while (1){
        buf = 0;
        LOGV("before read");
        ret = read(fd_r, &buf, sizeof(int32_t));
        if(ret > 0) {
            LOGV("[%s] read data ret[%d] buf[%d]", __FUNCTION__ , ret, buf);
            if (buf == SEND_START) {
                LOGV("[%s] read buf=1 from parent. start sign killer server.", __FUNCTION__ );
                int try_count = 5;
                while (pthread_create(&pthread, NULL, start_frida_inject_thread, data) && try_count > 0) {
                    sleep(1);
                    --try_count;
                }
            } else if (buf == SEND_STOP) {
                //主进程发送退出命令
                LOGV("[%s] read buf=2 from parent. stop sign killer server.", __FUNCTION__ );
                if(pthread != 0l) {
                    int kill_ret = pthread_kill(pthread, SIGKILL);
                    if(kill_ret == 0) {
                        pthread = 0l;
                    }
                }
            } else if (buf == SEND_STATUS) {
                LOGV("[%s] read buf=3 from parent. query sign killer server status : %s", __FUNCTION__ , str_bool(sign_kill_server_status));
                buf = sign_kill_server_status ? STATUS_ALIVE :STATUS_KILLED;
                write(fd_w, &buf, sizeof(int32_t));
            }
        } else {
            LOGV("[%s] read data null ret[%d] buf[%d]", __FUNCTION__ , ret, buf);
            sleep(5);
        }
    }
}

/**
 *
 * @param env
 *
 * 这块进程间通信有问题，暂停；
 */
void _init(JNIEnv* env) {
    pipe(sync_c);
    pipe(sync_p);

    pid_t parent_pid = getpid();
    pid_t pid = fork();
    if (pid > 0) {// parent

        close(sync_c[READ_PIPE]);
        close(sync_p[WRITE_PIPE]);

        thdata* data = (thdata*)malloc(sizeof(thdata));
        memset(data, 0, sizeof(thdata));
        data->fd_w = sync_c[WRITE_PIPE];
        data->fd_r = sync_p[READ_PIPE];
        data->pid = pid;

        int try_count = 5;
        pthread_t threadid2;
        while (pthread_create(&threadid2,NULL, wait_for_child, data) && try_count > 0) {
            LOGW("waring [%s] start keep_pipe_connect_parent thread error , sleep 1 second retry.", __FUNCTION__ );
            sleep(1);
            --try_count;
        }

    } else {//child

        prctl(PR_SET_NAME,"xyzggmanager_ch",NULL,NULL,NULL);

        close(sync_c[WRITE_PIPE]);
        close(sync_p[READ_PIPE]);
        thdata* data = (thdata*)malloc(sizeof(thdata));
        memset(data, 0, sizeof(thdata));
        data->fd_r = sync_c[READ_PIPE];
        data->fd_w = sync_p[WRITE_PIPE];
        data->pid = parent_pid;

        LOGV("thdata fdr:%d, fdw:%d, parent_pid:%d",data->fd_r, data->fd_w, data->pid);
//        int try_count = 5;
//        pthread_t threadid2;
//        while (pthread_create(&threadid2,NULL, keep_pipe_connect_child, data) && try_count > 0) {
//            LOGW("waring [%s] start keep_pipe_connect_parent thread error , sleep 1 second retry.", __FUNCTION__ );
//            sleep(1);
//            --try_count;
//        }

        keep_pipe_connect_child(data);
    }
}

void start_sign_kill_server() {
    int32_t buf;
    buf = SEND_START;
    write(sync_c[WRITE_PIPE], &buf, sizeof(int32_t));
}

void stop_sign_kill_server() {
    int32_t buf;
    buf = SEND_STOP;
    write(sync_c[WRITE_PIPE], &buf, sizeof(int32_t));
}

int check_sign_kill_server_status() {
//    char result[20];
//    exec_cmd("su -c \"ps | grep xyzggmanager_ch | wc -l\"", result, sizeof(result), false);
//    return strncmp(result, "1", strlen("1"));

    int32_t buf;
    buf = SEND_STATUS;
    write(sync_c[WRITE_PIPE], &buf, sizeof(int32_t));
    buf = 0;
    int ret = read(sync_p[READ_PIPE], &buf, sizeof(int32_t));
    if(ret > 0) {
        LOGV("[%s] ret[%d] buf[%d]",__FUNCTION__ ,ret, buf);
        return buf == STATUS_ALIVE;
    }
    return JNI_FALSE;
}