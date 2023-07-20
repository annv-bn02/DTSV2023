#ifndef __TIMER__
#define __TIMER__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x_gpio.h" 
#include "stm32f10x_tim.h"
#include "stdio.h"
extern unsigned char blink, blink_flag;
void Timer_Config(void);
/*---------------------------------------*/
//void Delay_1ms(void);
///*---------------------------------------*/
//void Delay_Ms(unsigned int time_ms);
///*---------------------------------------*/
//void Delay_1us(void);
///*---------------------------------------*/
//void Delay_Us(unsigned int time_us);
#ifdef __cplusplus
}
#endif

#endif
