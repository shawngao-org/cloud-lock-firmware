//
// Created by ShawnGao on 2023/7/31.
//

#ifndef CLOUD_LOCK_ESP8266_H
#define CLOUD_LOCK_ESP8266_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "delay.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "ini.h"
#include "math.h"
#include "time.h"

/**
 * [mqtt]
 * host=
 * port=
 * client_id=
 * user=
 * password=
 * subscription=
 * [wifi]
 * ssid=
 * password=
 */

#define TEST "AT\r\n"
#define RST "AT+RST\r\n"
#define RESTORE "AT+RESTORE\r\n"

#define SET_WIFI_MODE_1 "AT+CWMODE=1\r\n"
#define GET_WIFI_STATUS "AT+CWJAP?\r\n"
#define CONN_WIFI "AT+CWJAP=\"%s\",\"%s\"\r\n"

// #define TEST_WIFI "AT+CWRECONNCFG?\r\n"

#define SET_SNTP "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"
#define GET_TIME "AT+CIPSNTPTIME?\r\n"
#define GET_TIMESTAMP "AT+SYSTIMESTAMP?\r\n"

#define SET_USER_INFO "AT+MQTTUSERCFG=0,%d,\"%s\",\"%s\",\"%s\",0,0,\"%s\"\r\n"
#define CONN_MQTT "AT+MQTTCONN=0,\"%s\",%d,0\r\n"
#define MQTT_SUB "AT+MQTTSUB=0,\"%s\",1\r\n"
#define MQTT_PUB "AT+MQTTPUB=0,\"%s\",\"%s\",1,0\r\n"

void ESP8266_CLR();

_Bool ESP8266_WAIT_REV();

_Bool ESP8266_SEND_CMD(char *, char *);

void ESP8266_SEND_DATA(unsigned char *, unsigned short);

unsigned char *ESP8266_GET_IPD(unsigned short);

void ESP8266_INIT();

#endif //CLOUD_LOCK_ESP8266_H
