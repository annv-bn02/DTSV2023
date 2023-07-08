#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"
#include "debug.h"
#include "pwm.h"
#include "lcd.h"
#include "ILI9341_Gui.h"
#include "ILI9341_Driver.h"
#include "ILI9341_Define.h"
#include "keyboard.h"
#include "system.h"
void System_Config()
{



}
void System_Run()
{
		if(db_DEBUG_flag()==1)
		{
			db_DEBUG_Putchar("\n<---->\n");
			db_DEBUG_Putchar_RRX();
		}
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, blink);
		if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
		{
			
			NVIC_SystemReset();
		}
		Fn_DELAY_ms(1);
}