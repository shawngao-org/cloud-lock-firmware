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

void ESP8266_INIT() {
    HAL_UART_Receive_IT(&huart2, &esp8266_rx_byte, 1);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    USART1_Tx_String("Test ESP8266 ...");
    while (ESP8266_EXEC(TEST, "OK")) {
        delay_ms(500);
        USART1_Tx_String(".");
    }
    USART1_Tx_String("OK\n");
//    SET_WIFI();
    SET_TIME();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        esp8266_buf[esp8266_cnt] = esp8266_rx_byte;
        esp8266_cnt++;
        HAL_UART_Receive_IT(&huart2, &esp8266_rx_byte, 1);
    }
}
