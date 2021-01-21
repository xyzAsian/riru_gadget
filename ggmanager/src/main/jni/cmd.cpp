//
// Created by 1 on 2020/12/7.
//

#include <stdio.h>
#include <jni.h>
#include "log.h"
#include <string.h>


int exec_cmd(const char* cmd, char * result, int r_len, bool is_ret_status) {
    LOGV("cmd : %s, r_len : %d", cmd, r_len);
    FILE* fp = NULL;
    char _cmd[512];
    if(is_ret_status) {
        sprintf(_cmd, "%s>/dev/null; echo $?", cmd);
    } else {
        sprintf(_cmd, "%s", cmd);
    }
    if ((fp = popen(_cmd, "r")) != NULL) {
        memset(result, 0, r_len);
        char buf[r_len];
        while (fgets(buf, r_len, fp) != NULL) {
            LOGV("buf : %s, result : %s, len(result) : %d",buf , result, strlen(result));
            strncpy(result+strlen(result), buf, strlen(buf));
            memset(buf, 0 ,r_len);
        }
        pclose(fp);
    }
    //0 成功， 1 失败

    LOGV("result : %s",result);

    return 0;
}
