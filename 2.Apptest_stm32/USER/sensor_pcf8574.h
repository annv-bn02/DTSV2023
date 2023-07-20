#ifndef _SENSOR_PCF8574_H_
#define _SENSOR_PCF8574_H_

#include "stm32f10x.h"

#define PCF8574_ADDR 0x4E

#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3


#define	MODE_4_BIT		0x28
#define	CLR_SCR				0x01
#define	DISP_ON				0x0C
#define	CURSOR_ON			0x0E
#define	CURSOR_HOME		0x80

/*ham cau hinh timer*/
void Config_Timer(void);
/*---------------------------------------*/
void Delay_1ms(void);
/*---------------------------------------*/
void Delay_Ms(unsigned int time_ms);
/*---------------------------------------*/
void Delay_1us(void);
/*---------------------------------------*/
void Delay_Us(unsigned int time_us);

//void PCF8574_Init(void);
void PCF_Init(void);
void PCF_Backlight(uint8_t state) ;
void PCF_Goto(uint8_t row, uint8_t col);

void PCF_Clear(void);
void PCF_Print(char *str);
void PCF_SendCommand(char data);
void PCF_SendData(char data);


#endif







