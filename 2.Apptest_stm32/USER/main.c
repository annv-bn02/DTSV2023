#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"
#include "debug.h"
#include "timer.h"
#include "pwm.h"
//#include "lcd.h"
#include "mcu-i2c.h"
#include "sensor_pcf8574.h"
GPIO_InitTypeDef  GPIO_InitStructure;
void Fn_GPIO_Init (void);
int main (void){
	uint8_t blink_str;
	SystemCoreClockUpdate();
	Fn_DELAY_Init(72);
	Fn_GPIO_Init();
	db_DEBUG_Init(9600);
	Timer_Config();
	PWM_Config();
//	
//	Delay_Ms(100);
//	Lcd_Config();
//	Lcd_First_Config();
//	Delay_Ms(100);
//	lcd_Send_String_Line1("4");
	
	I2Cx_Init(I2C_USED, I2C1_B67, I2C_CLOCK_STANDARD);
	PCF_Init();
	PCF_Backlight(0);
	Fn_Delay_Ms(2000);
	PCF_Backlight(1);
	PCF_Print("Electronics");
	PCF_Goto(2,0);
	PCF_Print("PTIT");
	Fn_GPIO_Init();
	while(1)
	{
		if(db_DEBUG_flag()==1)
		{
			db_DEBUG_Putchar("\n<---->\n");
			db_DEBUG_Putchar_RRX();
		}
		if(blink && blink_flag)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			blink_flag = 0;
			PCF_Goto(2,6);
			PCF_Print("1");
		}
		else if(blink == 0 && blink_flag)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			blink_flag = 0;
			PCF_Goto(2,6);
			PCF_Print("0");
		}
		Fn_Delay_Ms(1);
	}
}


void Fn_GPIO_Init (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


