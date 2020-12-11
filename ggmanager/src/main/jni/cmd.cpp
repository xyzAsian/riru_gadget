//
// Created by 1 on 2020/12/7.
//

#include <stdio.h>
#include <jni.h>
#include "log.h"


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
//        while (fgets(result, r_len, fp) != NULL) {
//            LOGV("result : %s",result);
//        }
        fgets(result, r_len, fp);
        pclose(fp);
    }
    //0 成功， 1 失败

    LOGV("result : %s",result);

    return 0;
}
