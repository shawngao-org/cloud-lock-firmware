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

#define WIFI_SSID "OpenWrt_2.4G"
#define WIFI_PWD "network1925"

#define SERVER_HOST "mqtt.sgtu.ltd"
#define SERVER_PORT "1883"

#define TEST "AT\r\n"
#define RST "AT+RST\r\n"
#define RESTORE "AT+RESTORE\r\n"

#define SET_WIFI_MODE "AT+CWMODE=1\r\n"
#define ENABLE_SMART_CONFIG "AT+CWSTARTSMART\r\n"
#define DISABLE_SMART_CONFIG "AT+CWSTOPSMART\r\n"

#define SET_SNTP "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"

#define SET_USER_INFO "AT+MQTTUSERCFG=0,%d,\"%s\",\"%s\",\"%s\",0,0,\"%s\"\r\n"
#define CONN_MQTT "AT+MQTTCONN=0,\"%s\",%d,0\r\n"
#define MQTT_SUB "AT+MQTTSUB=0,\"%s\",1\r\n"
#define MQTT_PUB "AT+MQTTPUB=0,\"%s\",\"%s\",1,0\r\n"

#define REV_OK 0
#define REV_WAIT 1

void ESP8266_CLR();
_Bool ESP8266_WAIT_REV();
_Bool ESP8266_SEND_CMD(char *cmd, char *res);
void ESP8266_SEND_DATA(unsigned char *dat, unsigned short len);
unsigned char *ESP8266_GET_IPD(unsigned short timeout);
void ESP8266_INIT();

#endif //CLOUD_LOCK_ESP8266_H
