#include "main.h"

CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi1;

void SystemClock_Config(void);

static void MX_CRC_Init(void);

static void MX_GPIO_Init(void);

static void MX_SPI1_Init(void);

void OLED_display_home_page() {
    OLED_CLR();
    OLED_DISPLAY_ZH_CN(32,2,12);
    OLED_DISPLAY_ZH_CN(48,2,13);
    OLED_DISPLAY_ZH_CN(64,2,14);
    OLED_DISPLAY_ZH_CN(80,2,15);
}

void OLED_display_enter_pwd() {
    OLED_CLR();
    OLED_DISPLAY_ZH_CN(0,2,0);
    OLED_DISPLAY_ZH_CN(16,2,1);
    OLED_DISPLAY_ZH_CN(32,2,2);
    OLED_DISPLAY_ZH_CN(48,2,3);
    OLED_DISPLAY_ZH_CN(64,2,4);
    OLED_DISPLAY_ZH_CN(80,2,5);
    OLED_DISPLAY_CHAR(96,2,':',16);

    OLED_DISPLAY_CHAR(0,6,'[',16);
    OLED_DISPLAY_CHAR(8,6,'#',16);
    OLED_DISPLAY_CHAR(16,6,']',16);
    OLED_DISPLAY_ZH_CN(24,6,6);
    OLED_DISPLAY_ZH_CN(40,6,7);

    OLED_DISPLAY_CHAR(66,6,'[',16);
    OLED_DISPLAY_CHAR(74,6,'*',16);
    OLED_DISPLAY_CHAR(82,6,']',16);
    OLED_DISPLAY_ZH_CN(90,6,8);
    OLED_DISPLAY_ZH_CN(106,6,9);
}

void enter_pwd() {
    unsigned char key = 0xff;
    unsigned char pwd_chr_w = 0x08;
    unsigned char pwd_cur = 0;
    unsigned char pwd[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    OLED_display_enter_pwd();
    while (1) {
        key = TTP_SCAN();
        if ((key >= '0' && key <= '9') && pwd_cur <= 5) {
            OLED_DISPLAY_CHAR(pwd_cur * pwd_chr_w,4,key,16);
            pwd[pwd_cur] = key;
            delay_ms(100);
            key = 0;
            pwd_cur++;
        } else {
            if (key == '*') {
                if (pwd_cur > 0) {
                    pwd_cur--;
                } else {
                    return;
                }
                pwd[pwd_cur] = 0xff;
                OLED_DISPLAY_CHAR(pwd_cur * pwd_chr_w,4,' ',16);
                delay_ms(100);
                key = 0;
            }
        }
    }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    unsigned char key = 0xff;
    unsigned char CT[2];
    unsigned char SN[4];
    unsigned char status;

    HAL_Init();
    SystemClock_Config();
    MX_CRC_Init();
    MX_GPIO_Init();
    MX_SPI1_Init();

    delay_init();
    OLED_INIT();
    TTP_INIT();

    OLED_CLR();
    OLED_display_home_page();
    while (1) {
        key = TTP_SCAN();
        if ((key >= '0' && key <= '9') || key == '#') {
            enter_pwd();
            delay_ms(100);
            key = '*';
        }
        if (key == '*') {
            OLED_display_home_page();
        }
        delay_ms(100);
        key = 0;
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void) {

    /* USER CODE BEGIN CRC_Init 0 */

    /* USER CODE END CRC_Init 0 */

    /* USER CODE BEGIN CRC_Init 1 */

    /* USER CODE END CRC_Init 1 */
    hcrc.Instance = CRC;
    if (HAL_CRC_Init(&hcrc) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN CRC_Init 2 */

    /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void) {

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
