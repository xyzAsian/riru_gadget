//
// Created by 1 on 2020/11/23.
//

#include "utils.h"
#include "string.h"

char g_pkgname[128];

void read_process_name_from_cmdline(char* process_name, int pid) {
    char path[128];
    sprintf(path, "/proc/%d/cmdline", pid);
    FILE* cmdline_file =  fopen(path,"r");
    if(cmdline_file == NULL) return;

    fscanf(cmdline_file, "%s", process_name);
    fclose(cmdline_file);
}

int copy_file(const char* dest, const char * src){
    FILE *r_file = fopen (src,"rb");
    FILE *w_file = fopen (dest,"w");
    char temp[10];
    int judge = 0;
    if (r_file){
        do {
            judge = fread(temp,sizeof(char),10,r_file);
            if (!judge){
                break;
            }
            fwrite(temp,sizeof(char),10,w_file);
        }while(1);
    }
    fclose(r_file);
    fclose(w_file);
    r_file = NULL;
    w_file = NULL;
    return 0;
}


void str_strip(char * str) {
    if (str == NULL) {
        return;
    }
    int start = 0;
    while (start == 0) {
        if (*str == '\n' || *str == ' ') {
            str++;
        } else {
            start = 1;
        }
    }
    int end = 0;
    while (end == 0) {
        if (str[strlen(str)-1] == '\n' || str[strlen(str)-1] == ' ') {
            str[strlen(str)-1] = '\0';
        } else {
            end = 1;
        }
    }
}
