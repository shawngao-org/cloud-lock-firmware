//
// Created by 高新 on 2023/9/21.
//

#include "ini.h"

int getIniKey(FIL f, char *title, char *key, char *buf) {
    int flag = 0;
    char t[64], *tmp;
    char l[512];
    sprintf(t, "[%s]", title);
    while (f_gets(l, 512, &f) != NULL) {
        if (strncmp("//", l, 2) == 0) {
            continue;
        }
        if ('#' == l[0]) {
            continue;
        }
        tmp = strchr(l, '=');
        if (tmp != NULL && flag) {
            if (strncmp(key, l, strlen(key)) == 0) {
//                char *enter = strchr(l, 0x0a);
//                if (enter != NULL) {
//                    l[enter - l + 1] = '\0';
//                }
//                enter = strchr(l, 0x0d);
//                if (enter != NULL) {
//                    l[enter - l + 1] = '\0';
//                }
                if (l[strlen(l) - 2] == 0x0d) {
                    l[strlen(l) - 2] = '\0';
                } else {
                    if (l[strlen(l) - 1] == 0x0a) {
                        l[strlen(l) - 1] = '\0';
                    }
                }
                f_close(&f);
                while (*(tmp + 1) == ' ') {
                    tmp++;
                }
                strcpy(buf, tmp + 1);
                return 0;
            }
        } else {
            if (!strncmp(t, l, strlen(t))) {
                flag = 1;
            }
        }
    }
    f_close(&f);
    return -2;
}
