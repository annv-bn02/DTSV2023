#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"
#include "debug.h"
#include "stdio.h"
#include "flash_if.h"
#include "ota_uart_fram.h"
#include "usart.h"
#include "timer.h"
#include "watchdog.h"
#define LED GPIO_Pin_0

GPIO_InitTypeDef  GPIO_InitStructure;
void Fn_GPIO_Init (void);

typedef  void (*iapfun)(void);
void iap_load_app(u32 appxaddr);
iapfun jump2app;

__asm void MSR_MSP(uint32_t addr)
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

void iap_load_app(uint32_t appxaddr)
{
		int data = (*(volatile uint32_t*)appxaddr);
		jump2app = (iapfun)*(volatile uint32_t*)(appxaddr+4);     
		MSR_MSP(*(volatile uint32_t*)appxaddr);                   
		jump2app();
}

int main (void){
	int i, bootloader_stt_address = 0x0801C000;
	SystemInit();
	SystemCoreClockUpdate();
	Fn_GPIO_Init();
	Fn_DELAY_Init(72);
	db_DEBUG_Init(9600);
	USART_Config(9600);
	ETX_Receive_Chunk();
	Timer_Config();
	while(1)
	{
		mode = *(uint8_t *)bootloader_stt_address;
		if(mode)
		{
			ETX_Run();
		}
		else
		{
			iap_load_app(FLASH_APP_MAIN_ADDR);
		}
	}
}



void Fn_GPIO_Init (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
