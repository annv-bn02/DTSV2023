#include "mcu-i2c.h"
#include "sensor_pcf8574.h"

static uint8_t u8LCD_Buff[8];//bo nho dem luu lai toan bo
static uint8_t u8LcdTmp;

uint8_t backlightState = 1;
void Config_Timer(void){
	TIM_TimeBaseInitTypeDef TIMER;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIMER.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER.TIM_Period = 65535;
	TIMER.TIM_Prescaler = 1;
	TIMER.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIMER);
	
}



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void PCF_Init(void){
	/* Set 4-bits interface */
	PCF_SendCommand(0x33);		 
	Fn_Delay_Ms(10);
	PCF_SendCommand(0x32);
	Fn_Delay_Us(50);
	/* Start to set LCD function */
	PCF_SendCommand(0x28);
		Fn_Delay_Us(50);	
	/* clear LCD */
	PCF_SendCommand(0x01);
	/* wait 60ms */
		Fn_Delay_Ms(2);
	/* set entry mode */
	PCF_SendCommand(0x06);	Fn_Delay_Us(50);;
	/* set display to on */	
	PCF_SendCommand(0x0C);	Fn_Delay_Us(50);;
	/* move cursor to home and set data address to 0 */
	PCF_SendCommand(0x02);	Fn_Delay_Us(50);
}

void PCF_Goto(uint8_t row, uint8_t col) {
	switch (row){
		case 1:
			PCF_SendCommand(0x80 + col);
			Fn_Delay_Us(50);
			break;
		case 2:
			PCF_SendCommand(0x80 + col + 0x40);
			Fn_Delay_Us(50);
			break;
		case 3:
			PCF_SendCommand(0x80 + col + 0x14);
			Fn_Delay_Us(50);
			break;
		case 4:
			PCF_SendCommand(0x80 + col + 0x54);
			Fn_Delay_Us(50);
			break;
	}
}

void PCF_Backlight(uint8_t state) {
	backlightState = (state & 0x01) << LCD_BL;
	I2Cx_WriteByte(I2C_USED,PCF8574_ADDR,backlightState);
}

void PCF_SendData(char data){
	char data_u, data_l;
	uint8_t data_t[4],i=0;
	data_u = data&0xf0;
	data_l = (data<<4)&0xf0;
	data_t[0] = data_u|0x0d;  //en=1, rs=1, rw = 0 
	data_t[1] = data_u|0x09;  //en=0, rs=1,	rw = 0 
	data_t[2] = data_l|0x0d;  //en=1, rs=1, rw = 0 
	data_t[3] = data_l|0x09;  //en=0, rs=1, rw = 0 
	for(i = 0;i<4;i++){
		I2Cx_WriteByte(I2C_USED,PCF8574_ADDR,data_t[i]);
	}
}

void PCF_SendCommand(char data){
  char data_u, data_l;
	uint8_t data_t[4],i=0;
	data_u = data&0xf0;
	data_l = (data<<4)&0xf0;
	data_t[0] = data_u|0x04;  //en=1, rs=0, 
	data_t[1] = data_u;  			//en=0, rs=0,
	data_t[2] = data_l|0x04;  //en=1, rs=0,
	data_t[3] = data_l|0x08;  //en=0, rs=0,
	for(i = 0;i<4;i++){
		I2Cx_WriteByte(I2C_USED,PCF8574_ADDR,data_t[i]);
	}
}

void PCF_Print(char *str){
	while (*str) PCF_SendData(*str++);
}
void PCF_Clear(void){
	PCF_SendCommand(0x01);
	Fn_Delay_Ms(2);
}








