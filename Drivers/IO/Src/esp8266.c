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
    HAL_UART_Transmit(&huart4, (uint8_t *) cmd, strlen(cmd), HAL_MAX_DELAY);
    while (esp8266_flag == 1) {
        if (strstr((const char *) esp8266_buf, result)) {
            return 0;
        }
    }
    return 1;
}

void ESP8266_INIT() {
    HAL_UART_Receive_IT(&huart4, &esp8266_rx_byte, 1);
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    while (ESP8266_EXEC(TEST, "OK")) {
        delay_ms(500);
        HAL_UART_Transmit(&huart1, (uint8_t *)"Retrying...\r\n", 14, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t *)esp8266_buf, buf_size, HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)esp8266_buf, buf_size, HAL_MAX_DELAY);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        esp8266_buf[esp8266_cnt] = esp8266_rx_byte;
        esp8266_cnt++;
        HAL_UART_Receive_IT(&huart4, &esp8266_rx_byte, 1);
    }
}
