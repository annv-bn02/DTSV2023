
#include "stm32f10x.h"                  // Device header
	
#ifndef __ABC__
#define __ABC__

#ifdef __cplusplus
 extern "C" {
#endif
void Lcd_Config(void);
void Lcd_First_Config(void);
void lcd_Data_write(uint8_t data);
void lcd_Send_String_Line1(uint8_t *String);
void lcd_Send_String_Line2(uint8_t *String);
void clear_LCD(void);
void lcd_Control_write(uint8_t data);


#ifdef __cplusplus
}
#endif

#endif
