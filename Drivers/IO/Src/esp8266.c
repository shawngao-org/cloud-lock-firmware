//
// Created by ShawnGao on 2023/7/31.
//

#include "esp8266.h"

unsigned char esp8266_buf[128];
unsigned char esp8266_rx_byte;
int buf_size = 128;
unsigned short esp8266_cnt = 0;
unsigned short esp8266_flag;

void ESP8266_CLR() {
    memset(esp8266_buf, 0, buf_size);
    esp8266_cnt = 0;
}

int ESP_RD_INI_CONF(char *title, char *key, char *buf) {
    return getIniKey(CONF_FILE, title, key, buf);
}

int ESP8266_EXEC(char *cmd, char *result) {
    ESP8266_CLR();
    USART2_Tx_String(cmd);
    while (esp8266_flag == 1) {
        if (strstr((const char *) esp8266_buf, result)) {
            return 0;
        }
    }
    return 1;
}

void SET_TIME() {
    USART1_Tx_String("Set NTP ...");
    while (ESP8266_EXEC(SET_SNTP, "OK")) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String("OK\n");
    USART1_Tx_String("Get Time ...");
    while (ESP8266_EXEC(GET_TIME, "OK") && strstr((char *) esp8266_buf, "1970") == NULL) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String("\n");
    while (ESP8266_EXEC(GET_TIMESTAMP, "OK")) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String((char *) esp8266_buf);
    unsigned char timestamp_str[11];
    for (int i = 0, j = 0; esp8266_buf[i] != '\0'; ++i) {
        if (esp8266_buf[i] >= '0' && esp8266_buf[i] <= '9' && j < 10) {
            timestamp_str[j++] = esp8266_buf[i];
        }
    }
    timestamp_str[10] = '\0';
    time_t timestamp = 0;
    int len = 9;
    for (int i = len; i >= 0; --i) {
        if (timestamp_str[i] >= '0' && timestamp_str[i] <= '9') {
            timestamp += pow(10 * 1.0, (len - i) * 1.0) * (timestamp_str[i] - '0');
        }
    }
    timestamp += 8 * 60 * 60;
    struct tm tm_t = *localtime(&timestamp);
    RTC_DateTypeDef rtcDateTypeDef;
    RTC_TimeTypeDef rtcTimeTypeDef;
    rtcDateTypeDef.Year = tm_t.tm_year - 100;
    rtcDateTypeDef.Month = tm_t.tm_mon + 1;
    rtcDateTypeDef.Date = tm_t.tm_mday;
    rtcTimeTypeDef.Hours = tm_t.tm_hour;
    rtcTimeTypeDef.Minutes = tm_t.tm_min;
    rtcTimeTypeDef.Seconds = tm_t.tm_sec + 5;
    HAL_RTC_SetTime(&hrtc, &rtcTimeTypeDef, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &rtcDateTypeDef, RTC_FORMAT_BIN);
    USART1_Tx_String("\n");
    USART1_Tx_String("OK\n");
}

void SET_WIFI() {
    USART1_Tx_String("Set WiFi Mode 1 ...");
    while (ESP8266_EXEC(SET_WIFI_MODE_1, "OK")) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String("OK\n");
    USART1_Tx_String("Get WiFi Status ...");
    while (ESP8266_EXEC(GET_WIFI_STATUS, "OK")) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String("OK\n");
    if (strstr((char *) esp8266_buf, "+CWJAP")) {
        USART1_Tx_String("WiFi config ...");
        char conn_wifi_str[128];
        char ssid[32];
        if (ESP_RD_INI_CONF("wifi", "ssid", ssid)) {
            Error_Handler();
        }
        char passwd[32];
        if (ESP_RD_INI_CONF("wifi", "password", passwd)) {
            Error_Handler();
        }
        sprintf(conn_wifi_str, CONN_WIFI, ssid, passwd);
        while (ESP8266_EXEC(conn_wifi_str, "GOT IP")) {
            USART1_Tx_String(".");
            delay_ms(500);
        }
        USART1_Tx_String("OK\n");
    }
}

void ESP8266_INIT() {
    HAL_UART_Receive_IT(&huart2, &esp8266_rx_byte, 1);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    USART1_Tx_String("Test ESP8266 ...");
    while (ESP8266_EXEC(TEST, "OK")) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String("OK\n");
    SET_WIFI();
    SET_TIME();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        esp8266_buf[esp8266_cnt] = esp8266_rx_byte;
        esp8266_cnt++;
        HAL_UART_Receive_IT(&huart2, &esp8266_rx_byte, 1);
    }
}
