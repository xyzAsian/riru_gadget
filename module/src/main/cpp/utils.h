//
// Created by 1 on 2020-03-09.
//

#ifndef XPOSEHOOKDEMO_LOG_H
#define XPOSEHOOKDEMO_LOG_H

#include <android/log.h>
#include <stdio.h>

extern char g_pkgname[128];

#if 1

#define LOG_TAG "riru_signkill"
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

#else

#define LOGV(...)
#define LOGE(...)

#endif

void read_process_name_from_cmdline(char* process_name, int pid);
int copy_file(const char* dest, const char * src);
void str_strip(char * str);

#endif //XPOSEHOOKDEMO_LOG_H
