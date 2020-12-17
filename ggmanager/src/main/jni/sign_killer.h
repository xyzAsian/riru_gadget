//
// Created by 1 on 2020/12/8.
//

#ifndef RIRU_GADGET_SIGN_KILLER_H
#define RIRU_GADGET_SIGN_KILLER_H
#include <jni.h>

void _init(JNIEnv* env);

void start_sign_kill_server();

void stop_sign_kill_server();

int check_sign_kill_server_status();

#endif //RIRU_GADGET_SIGN_KILLER_H
