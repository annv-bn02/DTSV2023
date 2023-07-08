#include "ILI9341_Define.h"

void TFT_SPI_Display_Init(void){
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*CS pin configuration*/
	RCC_APB2PeriphClockCmd(RCC_TFT_PORT_CS,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = TFT_PIN_CS; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(TFT_PORT_CS, &GPIO_InitStructure);
	/*DC pin configuration*/
	RCC_APB2PeriphClockCmd(RCC_TFT_PORT_DC,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = TFT_PIN_DC; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(TFT_PORT_DC, &GPIO_InitStructure);
	
	/*RST pin configuration*/
	RCC_APB2PeriphClockCmd(RCC_TFT_PORT_RST,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = TFT_PIN_RST; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(TFT_PORT_RST, &GPIO_InitStructure);
	
	/*SPI: MOSI/SCK pin configuration*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_TFT_SPI_PORT, ENABLE);//
	GPIO_InitStructure.GPIO_Pin = TFT_PIN_SCK | TFT_PIN_MOSI;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(TFT_SPI_PORT, &GPIO_InitStructure);//
	/*SPI:MISO pin configuration*/
	GPIO_InitStructure.GPIO_Pin =  TFT_PIN_MISO;//    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(TFT_SPI_PORT, &GPIO_InitStructure);//  
/*----------------------------------------------------------------*/	
	
	/*SPI MODE configuration*/
	RCC_APB1PeriphClockCmd(RCC_TFT_SPI_DISPLAY ,ENABLE);//   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(TFT_SPI_DISPLAY, &SPI_InitStructure);//
	SPI_Cmd(TFT_SPI_DISPLAY, ENABLE);   //
}

