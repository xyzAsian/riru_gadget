//
// Created by 1 on 2020/12/8.
//

#ifndef RIRU_GADGET_SIGN_KILLER_H
#define RIRU_GADGET_SIGN_KILLER_H
#include <jni.h>

void start_kill_signcheck(JNIEnv* env);

void stop_kill_signcheck();

int is_child_alive();

#endif //RIRU_GADGET_SIGN_KILLER_H
