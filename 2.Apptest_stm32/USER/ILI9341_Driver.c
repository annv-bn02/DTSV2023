#include "ILI9341_Driver.h"
#include "pwm.h"
#include "ILI9341_Define.h"
#include "ILI9341_Gui.h"

TFT_dev ili_dev;
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;
void TFT_Init(void){
	TFT_SPI_Display_Init();

	/* Reset The Screen */
	TFT_Reset();
	TFT_Send_Command(0xCF);  
	TFT_Send_Data(0x00); 
	TFT_Send_Data(0xD9); //C1 
	TFT_Send_Data(0X30); 
	TFT_Send_Command(0xED);  
	TFT_Send_Data(0x64); 
	TFT_Send_Data(0x03); 
	TFT_Send_Data(0X12); 
	TFT_Send_Data(0X81); 
	TFT_Send_Command(0xE8);  
	TFT_Send_Data(0x85); 
	TFT_Send_Data(0x10); 
	TFT_Send_Data(0x7A); 
	TFT_Send_Command(0xCB);  
	TFT_Send_Data(0x39); 
	TFT_Send_Data(0x2C); 
	TFT_Send_Data(0x00); 
	TFT_Send_Data(0x34); 
	TFT_Send_Data(0x02); 
	TFT_Send_Command(0xF7);  
	TFT_Send_Data(0x20); 
	TFT_Send_Command(0xEA);  
	TFT_Send_Data(0x00); 
	TFT_Send_Data(0x00); 
	TFT_Send_Command(0xC0);    //Power control 
	TFT_Send_Data(0x1B);   //VRH[5:0] 
	TFT_Send_Command(0xC1);    //Power control 
	TFT_Send_Data(0x12);   //SAP[2:0];BT[3:0] //0x01
	TFT_Send_Command(0xC5);    //VCM control 
	TFT_Send_Data(0x26); 	 //3F
	TFT_Send_Data(0x26); 	 //3C
	TFT_Send_Command(0xC7);    //VCM control2 
	TFT_Send_Data(0XB0); 
	TFT_Send_Command(0x36);    // Memory Access Control 
	TFT_Send_Data(0x08); 
	TFT_Send_Command(0x3A);   
	TFT_Send_Data(0x55); 
	TFT_Send_Command(0xB1);   
	TFT_Send_Data(0x00);   
	TFT_Send_Data(0x1A); 
	TFT_Send_Command(0xB6);    // Display Function Control 
	TFT_Send_Data(0x0A); 
	TFT_Send_Data(0xA2); 
	TFT_Send_Command(0xF2);    // 3Gamma Function Disable 
	TFT_Send_Data(0x00); 
	TFT_Send_Command(0x26);    //Gamma curve selected 
	TFT_Send_Data(0x01); 
	TFT_Send_Command(0xE0); //Set Gamma
	TFT_Send_Data(0x1F);
	TFT_Send_Data(0x24);
	TFT_Send_Data(0x24);
	TFT_Send_Data(0x0D);
	TFT_Send_Data(0x12);
	TFT_Send_Data(0x09);
	TFT_Send_Data(0x52);
	TFT_Send_Data(0xB7);
	TFT_Send_Data(0x3F);
	TFT_Send_Data(0x0C);
	TFT_Send_Data(0x15);
	TFT_Send_Data(0x06);
	TFT_Send_Data(0x0E);
	TFT_Send_Data(0x08);
	TFT_Send_Data(0x00);
	TFT_Send_Command(0XE1); //Set Gamma
	TFT_Send_Data(0x00);
	TFT_Send_Data(0x1B);
	TFT_Send_Data(0x1B);
	TFT_Send_Data(0x02);
	TFT_Send_Data(0x0E);
	TFT_Send_Data(0x06);
	TFT_Send_Data(0x2E);
	TFT_Send_Data(0x48);
	TFT_Send_Data(0x3F);
	TFT_Send_Data(0x03);
	TFT_Send_Data(0x0A);
	TFT_Send_Data(0x09);
	TFT_Send_Data(0x31);
	TFT_Send_Data(0x37);
	TFT_Send_Data(0x1F);

	TFT_Send_Command(0x2B); 
	TFT_Send_Data(0x00);
	TFT_Send_Data(0x00);
	TFT_Send_Data(0x01);
	TFT_Send_Data(0x3f);
	TFT_Send_Command(0x2A); 
	TFT_Send_Data(0x00);
	TFT_Send_Data(0x00);
	TFT_Send_Data(0x00);
	TFT_Send_Data(0xef);	 
	TFT_Send_Command(0x11); //Exit Sleep
	Delay_Ms(120);
	TFT_Send_Command(0x29); //display on	
	TFT_Send_Data(0x2C);
	TFT_Gui_Direction(3);
}

void TFT_Reset(void){
	TFT_PIN_RST_RESET;
	Delay_Ms(50);
	TFT_PIN_RST_SET;
	Delay_Ms(50);
//	TFT_Send_Command(0x01);
}

u8 SPI_SendByte(u8 Byte){
	while((TFT_SPI_DISPLAY->SR&SPI_I2S_FLAG_TXE)==RESET);			  
	TFT_SPI_DISPLAY->DR=Byte;	 
	while((TFT_SPI_DISPLAY->SR&SPI_I2S_FLAG_RXNE)==RESET);  
	return TFT_SPI_DISPLAY->DR;          	     	
} 

static void TFT_Send_Command(unsigned char command){
	TFT_PIN_CS_RESET;
	TFT_PIN_DC_RESET;	
	SPI_SendByte(command);
	TFT_PIN_CS_SET;
}

static void TFT_Send_Data(unsigned char data){
	TFT_PIN_CS_RESET;
	TFT_PIN_DC_SET;
	SPI_SendByte(data);
	TFT_PIN_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void TFT_WriteRAM_Prepare(void){
	TFT_Send_Command(ili_dev.wramcmd);
}	  

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void TFT_WriteReg(u8 LCD_Reg, u16 LCD_RegValue){	
	TFT_Send_Command(LCD_Reg);  
	TFT_Send_Data(LCD_RegValue);	    		 
}	

void TFT_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd){	
	TFT_Send_Command(ili_dev.setxcmd);	
	TFT_Send_Data(xStar>>8);
	TFT_Send_Data(0x00FF&xStar);		
	TFT_Send_Data(xEnd>>8);
	TFT_Send_Data(0x00FF&xEnd);

	TFT_Send_Command(ili_dev.setycmd);	
	TFT_Send_Data(yStar>>8);
	TFT_Send_Data(0x00FF&yStar);		
	TFT_Send_Data(yEnd>>8);
	TFT_Send_Data(0x00FF&yEnd);

	TFT_WriteRAM_Prepare();	//¿ªÊ¼Ð´ÈëGRAM			
} 

void TFT_Send_Data_16Bit(u16 Data){	
	TFT_PIN_CS_RESET;
	TFT_PIN_DC_SET;  
	SPI_SendByte(Data>>8);
	SPI_SendByte(Data);
	TFT_PIN_CS_SET;
}