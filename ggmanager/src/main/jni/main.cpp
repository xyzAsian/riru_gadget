//
// Created by 1 on 2020/12/8.
//

#include <jni.h>
#include "log.h"
#include "cmd.h"
#include "sign_killer.h"

JNIEXPORT
JNICALL
extern "C"
jstring exec(JNIEnv* env, jclass clazz, jstring cmd, jboolean is_ret_status) {

    const char * _cmd = env->GetStringUTFChars(cmd, NULL);

    char result[1024];
    exec_cmd(_cmd, result, sizeof(result), (bool)is_ret_status);

    return env->NewStringUTF(result);
}

JNIEXPORT
JNICALL
extern "C"
void start_kill_sign(JNIEnv* env, jclass clazz){
    LOGV("[%s]",__FUNCTION__ );
    start_kill_signcheck(env);
}


JNIEXPORT
JNICALL
extern "C"
void stop_kill_sign(JNIEnv* env, jclass clazz) {
    LOGV("[%s]",__FUNCTION__ );
    stop_kill_signcheck();
}

JNIEXPORT
JNICALL
extern "C"
jboolean is_sign_alive(JNIEnv* env, jclass clazz) {
    LOGV("[%s]",__FUNCTION__ );
    return is_child_alive() == 0;
}

static inline void setJniMethodTable(const char *name, const char *args, void *func, JNINativeMethod *method) {
    method->name = name;
    method->signature = args;
    method->fnPtr = func;
}


static inline void registerNativeMethod(JNIEnv* env) {
    LOGV("start regist");
    jclass nativelib = env->FindClass("com/gadget/manager/utils/NativeLib");
    JNINativeMethod methods[4];
    setJniMethodTable("exec","(Ljava/lang/String;Z)Ljava/lang/String;", (void*)exec,&methods[0]);
    setJniMethodTable("startSignKillerServer","()V", (void*)start_kill_sign,&methods[1]);
    setJniMethodTable("stopSignKillerServer","()V", (void*)stop_kill_sign,&methods[2]);
    setJniMethodTable("isSignKillerProcessAlive","()Z", (void*)is_sign_alive,&methods[3]);
    env->RegisterNatives(nativelib, methods, sizeof(methods)/sizeof(methods[0]));
    LOGV("end regist");
}


JNIEXPORT
extern "C"
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_ERR;
    }

    registerNativeMethod(env);

    return JNI_VERSION_1_4;
}
