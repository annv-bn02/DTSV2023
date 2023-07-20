#include "stm32f10x.h"                  // Device header
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "pwm.h"
#include "lcd.h"

#define PCF8574_Address      	0x4e



void Lcd_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0;
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_Cmd(I2C1,ENABLE); 	
}

//giao tiep tung bai voi LCD
void lcd_Write_byte(uint8_t data)
{
	I2C_GenerateSTART( I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,PCF8574_Address,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1,data);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP( I2C1,ENABLE);
}
//write lenh control LCD
void lcd_Control_write(uint8_t data)
{
	uint8_t data_u,data_1;
	uint8_t data_t[4],i=0;
	data_u = data&0xf0;
	data_1 = (data<<4)&0xf0;
	data_t[0] = data_u|0x0c;
	data_t[1] = data_u|0x08;
	data_t[2] = data_1|0x0c;
	data_t[3] = data_1|0x08;
	for(i =0 ;i<4;i++)
	{
		lcd_Write_byte(data_t[i]);
	}
}
//write du lieu vao LCD
void lcd_Data_write(uint8_t data)
{
	uint8_t data_u,data_1;
	uint8_t data_t[4],i=0;
	data_u = data&0xf0;
	data_1 = (data<<4)&0xf0;
	data_t[0] = data_u|0x0d;
	data_t[1] = data_u|0x09;
	data_t[2] = data_1|0x0d;
	data_t[3] = data_1|0x09;
	for(i =0 ;i<4;i++)
	{
		lcd_Write_byte(data_t[i]);
	}
}
//khoi tao LCD
void Lcd_First_Config(void)
{
	lcd_Control_write(0x33);
	Fn_Delay_Ms(10);
	lcd_Control_write(0x32);
	Fn_Delay_Ms(50);
	lcd_Control_write(0x28);
	Fn_Delay_Ms(50);
	lcd_Control_write(0x01);
	Fn_Delay_Ms(50);
	
	lcd_Control_write(0x06);
	Fn_Delay_Ms(50);
	lcd_Control_write(0x0c);
	Fn_Delay_Ms(50);
	lcd_Control_write(0x02);
	Fn_Delay_Ms(50);
}
//hien thi dong 1 lcd
void lcd_Send_String_Line1(uint8_t *String)
{
	while(*String)
	{
		lcd_Data_write(*String);
		String++;
	}
}
//hien thi dong 2 lcd
void lcd_Send_String_Line2(uint8_t *String)
{
	while(*String)
	{
		lcd_Data_write(*String);
		String++;
	}
}
//clear man hinh
void clear_LCD(void)
{
	lcd_Control_write(0x01);
	Fn_Delay_Ms(10);
}
