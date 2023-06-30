/*
  ******************************************************************************
  * @file		usart.h                                                              *
  * @author	    Nguyen Van An                                                *
  * @date		23/04/2022                                                      *
  ******************************************************************************
*/
	
#ifndef __USART__
#define __USART__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x_usart.h"  
#include "stdio.h"
extern char string_receive[1000];
uint8_t USART_FlagDB(void);  //co xac nhan da nhan xong
uint8_t USART_Compare(char *string); //ham so sanh chuoi nhan duoc voi chuoi string do minh nhap 
void USART_SendChar(char vr_Char); //ham gui di 1 ky tu
void USART_SendString(char *vr_Str); //ham gui di 1 chuoi
void USART_SendNum(int num); //ham gui di 1 so
void USART_Config(uint16_t Baurates); //ham congif uart
#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
