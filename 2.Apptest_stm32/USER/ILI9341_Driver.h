#ifndef _ILI9341_Driver_H_
#define _ILI9341_Driver_H_

#include <stm32f10x.h>
#include "ILI9341_Define.h"
#include <stdlib.h>

#define  CMD_RESET_OFF 		GPIO_WriteBit(TFT_PORT_RST,TFT_PIN_RST,Bit_SET)
#define  CMD_RESET_ON 		GPIO_WriteBit(TFT_PORT_RST,TFT_PIN_RST,Bit_RESET)
#define  CMD_CS_OFF 			GPIO_WriteBit(TFT_PORT_CS,TFT_PIN_CS,Bit_SET)
#define  CMD_CS_ON 				GPIO_WriteBit(TFT_PORT_CS,TFT_PIN_CS,Bit_RESET)
#define  CMD_DC_DATA 			GPIO_WriteBit(TFT_PORT_DC,TFT_PIN_DC,Bit_SET)
#define  CMD_DC_COMMAND 	GPIO_WriteBit(TFT_PORT_DC,TFT_PIN_DC,Bit_RESET)

#define BLACK       	0x0000
#define NAVY        	0x000F
#define DARKGREEN   	0x03E0
#define DARKCYAN    	0x03EF
#define MAROON      	0x7800
#define PURPLE      	0x780F
#define OLIVE       	0x7BE0
#define LIGHTGREY   	0xC618
#define DARKGREY    	0x7BEF
#define BLUE        	0x001F
#define GREEN       	0x07E0
#define CYAN        	0x07FF
#define RED         	0xF800
#define MAGENTA     	0xF81F
#define YELLOW      	0xFFE0
#define WHITE       	0xFFFF
#define ORANGE      	0xFD20
#define GREENYELLOW 	0xAFE5
#define PINK        	0xF81F

#define BOLD 					1 // in dam
#define NORMAL				0 // in thuong

#define swap(a,b) 		{int16_t t=a;a=b;b=t;}
#define BURST_MAX_SIZE 	500

#define TFT_W 240
#define TFT_H 320

extern u16  POINT_COLOR;   
extern u16  BACK_COLOR;

typedef struct  
{										    
	u16 width;			
	u16 height;			
	u16 id;				  
	u8  dir;			  
	u16	 wramcmd;		
	u16  setxcmd;		
	u16  setycmd;		 
}TFT_dev; 	

extern TFT_dev ili_dev;
void TFT_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void TFT_WriteRAM_Prepare(void);


void Show_Menu(void);
void TFT_Init(void);
void TFT_Reset(void);
u8 SPI_SendByte(u8 Byte);

static void TFT_Send_Command(unsigned char command);
static void TFT_Send_Data(unsigned char data);
void TFT_Send_Data_16Bit(u16 Data);
void TFT_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
	
void TFT_Set_Rotation(unsigned char rotation);
void TFT_Fill_Screen(unsigned int color);

void TFT_Draw_Pixel(int x, int y, unsigned int color);
void TFT_Draw_Double_Pixel(int x, int y, unsigned int color1, unsigned int color2);
void TFT_Draw_Circle(unsigned int x0, unsigned int y0, int r, unsigned int color, unsigned char flood);
void TFT_Draw_Line(unsigned int color, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void TFT_Draw_Filled_Rectangle(unsigned int color,unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void TFT_Draw_Empty_Rectangle(unsigned int color,unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void TFT_Draw_String(unsigned int x, unsigned int y, unsigned int color, unsigned int phone, char *str, unsigned char size);
void Text_Center(u16 x1, u16 y1, u16 x2, u16 y2, u16 fc, u16 bc, u8 size, u8 type_text, u8 *str,u8 mode);
#endif
