//
// Created by 高新 on 2023/9/21.
//

#ifndef CLOUD_LOCK_INI_H
#define CLOUD_LOCK_INI_H

#include "stdio.h"
#include "string.h"
#include "ff.h"

int getIniKey(FIL f, char *title, char *key, char *buf);

#endif //CLOUD_LOCK_INI_H
