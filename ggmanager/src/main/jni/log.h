//
// Created by 1 on 2020/12/7.
//

#ifndef RIRU_GADGET_LOG_H
#define RIRU_GADGET_LOG_H

#include <android/log.h>

//#define LOG_TAG "ggmanager"

#ifdef LOG_TAG
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#else
#define LOGV(...)
#define LOGW(...)
#define LOGE(...)
#endif // LOG_TAG

#endif //RIRU_GADGET_LOG_H
