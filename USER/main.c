#include "main.h"

void OLED_display_home_page() {
	OLED_Clear();
	OLED_ShowCHinese(32,2,12);
	OLED_ShowCHinese(48,2,13);
	OLED_ShowCHinese(64,2,14);
	OLED_ShowCHinese(80,2,15);
}

void OLED_display_enter_pwd() {
	OLED_Clear();
	OLED_ShowCHinese(0,2,0);
	OLED_ShowCHinese(16,2,1);
	OLED_ShowCHinese(32,2,2);
	OLED_ShowCHinese(48,2,3);
	OLED_ShowCHinese(64,2,4);
	OLED_ShowCHinese(80,2,5);
	OLED_ShowChar(96,2,':',16);
	
	OLED_ShowChar(0,6,'[',16);
	OLED_ShowChar(8,6,'#',16);
	OLED_ShowChar(16,6,']',16);
	OLED_ShowCHinese(24,6,6);
	OLED_ShowCHinese(40,6,7);
	
	OLED_ShowChar(66,6,'[',16);
	OLED_ShowChar(74,6,'*',16);
	OLED_ShowChar(82,6,']',16);
	OLED_ShowCHinese(90,6,8);
	OLED_ShowCHinese(106,6,9);
}

int main(void) {
	u8 key = 0xff;
	unsigned char CT[2];
  unsigned char SN[4];
	unsigned char status;
	unsigned char pwd_chr_w = 0x08;
	unsigned char pwd_cur = 0;
  unsigned char pwd[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	
	// GPIO_InitTypeDef GPIO_InitStructure;
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	// GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	// //I2C1_SCL->PB8    //I2C1_SDA->PB9
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
	uart_init(115200);
	printf("Initialized the UART serial baud rate: 115200 Baud\r\n");
	printf("Start initializing the delayer...\r");
	delay_init();
	printf("[OK]\r\n");
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	printf("Start initializing the NVIC configure...\r");
	NVIC_Configuration();
	printf("[OK]\r\n");
	printf("Start initializing the OLED...\r");
	OLED_Init();		
	printf("[OK]\r\n");
	printf("Start initializing the RFID-RC522...\r");
	RC522_Init();
	printf("[OK]\r\n");
	printf("Start displaying home page on the OLED...\r");
	OLED_display_home_page();
	printf("[OK]\r\n");
	printf("Start initializing the touch keypad...\r");
	Touch_Configuration();
	printf("[OK]\r\n");
	printf("System started successfully!\r\n");
	while(1) {
		status = PcdRequest(PICC_REQALL,CT);
		if (status == MI_OK) {
		  OLED_ShowChar(120,0,status,16);
			status = MI_ERR;
			status = PcdAnticoll(SN);
	    OLED_ShowCHinese(0,0,10);
	    OLED_ShowCHinese(16,0,11);
			OLED_ShowChar(32,0,':', 16);
	    OLED_ShowHex(40,0,SN[0] >> 4);
	    OLED_ShowHex(48,0,SN[0] & 0x0f);
	    OLED_ShowHex(56,0,SN[1] >> 4);
	    OLED_ShowHex(64,0,SN[1] & 0x0f);
	    OLED_ShowHex(72,0,SN[2] >> 4);
	    OLED_ShowHex(80,0,SN[2] & 0x0f);
	    OLED_ShowHex(88,0,SN[3] >> 4);
	    OLED_ShowHex(96,0,SN[3] & 0x0f);
			WaitCardOff();
		} else {
		  OLED_ShowChar(120,0,' ',16);
			OLED_ShowChar(0,0,' ', 16);
			OLED_ShowChar(8,0,' ', 16);
			OLED_ShowChar(16,0,' ', 16);
			OLED_ShowChar(24,0,' ', 16);
			OLED_ShowChar(32,0,' ', 16);
			OLED_ShowChar(40,0,' ', 16);
	    OLED_ShowChar(48,0,' ', 16);
	    OLED_ShowChar(56,0,' ', 16);
	    OLED_ShowChar(64,0,' ', 16);
	    OLED_ShowChar(72,0,' ', 16);
	    OLED_ShowChar(80,0,' ', 16);
	    OLED_ShowChar(88,0,' ', 16);
	    OLED_ShowChar(96,0,' ', 16);
		}
		key = Touch_KeyScan();
		if ((key >= '0' && key <= '9') || key == '#') {
			OLED_display_enter_pwd();
		}
		if ((key >= '0' && key <= '9') && pwd_cur <= 5) {
			OLED_ShowChar(pwd_cur * pwd_chr_w,4,key,16);
			pwd[pwd_cur] = key;
			delay_ms(100);
			key = 0;
			pwd_cur++;
		} else {
			if (key == '*') {
			  if (pwd_cur > 0) {
					pwd_cur--;
				} else {
					OLED_display_home_page();
				}
			  pwd[pwd_cur] = 0xff;
			  OLED_ShowChar(pwd_cur * pwd_chr_w,4,' ',16);
				delay_ms(100);
			  key = 0;
			}
		}
	}
}
