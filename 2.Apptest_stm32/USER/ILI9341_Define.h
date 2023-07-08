#ifndef _ILI9341_DEFINE_H_
#define _ILI9341_DEFINE_H_
#include "stm32f10x.h"


/* USER CODE BEGIN Private defines */

#define NUM_PIN_DC								8
#define TFT_PIN_DC	 					GPIO_Pin_8
#define TFT_PORT_DC	 					GPIOB
#define RCC_TFT_PORT_DC				RCC_APB2Periph_GPIOB

#define NUM_PIN_CS								9
#define TFT_PIN_CS 						GPIO_Pin_9
#define TFT_PORT_CS 					GPIOB
#define RCC_TFT_PORT_CS				RCC_APB2Periph_GPIOB

#define NUM_PIN_RST								12
#define TFT_PIN_RST 					GPIO_Pin_12
#define TFT_PORT_RST	 				GPIOB
#define RCC_TFT_PORT_RST			RCC_APB2Periph_GPIOB

#define TFT_SPI_DISPLAY				SPI2
#define TFT_SPI_PORT					GPIOB
#define RCC_TFT_SPI_DISPLAY   RCC_APB1Periph_SPI2
#define RCC_TFT_SPI_PORT			RCC_APB2Periph_GPIOB
#define TFT_PIN_SCK 					GPIO_Pin_13
#define TFT_PIN_MISO 					GPIO_Pin_14
#define TFT_PIN_MOSI 					GPIO_Pin_15

#define	TFT_PIN_CS_SET  			TFT_PORT_CS->BSRR  = 1<<NUM_PIN_CS    
#define	TFT_PIN_DC_SET				TFT_PORT_DC->BSRR  = 1<<NUM_PIN_DC      
#define	TFT_PIN_RST_SET				TFT_PORT_RST->BSRR = 1<<NUM_PIN_RST   
						    
#define	TFT_PIN_CS_RESET  		TFT_PORT_CS->BRR   = 1<<NUM_PIN_CS     
#define	TFT_PIN_DC_RESET			TFT_PORT_DC->BRR   = 1<<NUM_PIN_DC    
#define	TFT_PIN_RST_RESET			TFT_PORT_RST->BRR  = 1<<NUM_PIN_RST 

/*------------------------------------------------------------------------*/
//#define TFT_SPI_TOUCHSCREEN		SPI1	

//#define TFT_TP_PIN_IRQ 				GPIO_Pin_3
//#define TFT_TP_PORT_IRQ	 			GPIOA

//#define TFT_TP_PIN_CS 				GPIO_Pin_4
//#define TFT_TP_PORT_CS	 			GPIOA

//#define TFT_TP_PIN_SCK 				GPIO_Pin_5
//#define TFT_TP_PORT_SCK	 			GPIOA

//#define TFT_TP_PIN_MISO 			GPIO_Pin_6
//#define TFT_TP_PORT_MISO	 		GPIOA

//#define TFT_TP_PIN_MOSI 			GPIO_Pin_7
//#define TFT_TP_PORT_MOSI	 		GPIOA

/*------------------------------------------------------------------------*/
//#define TFT_PIN_ 					GPIO_Pin_
//#define TFT_PORT_	 				GPIO

//#define TFT_PIN_ 					GPIO_Pin_
//#define TFT_PORT_	 				GPIO

//#define TFT_PIN_ 					GPIO_Pin_
//#define TFT_PORT_	 				GPIO

//#define TFT_PIN_ 					GPIO_Pin_
//#define TFT_PORT_	 				GPIO


#define READ_X 				0xD0
#define READ_Y 				0x90

void TFT_SPI_Display_Init(void);

#endif

