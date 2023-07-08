/*
  ******************************************************************************
  * @file		pwm.h                                                              *
  * @author	    Nguyen Van An                                               *
  * @date		23/04/2022                                                      *
  ******************************************************************************
*/
	
#ifndef __GPIO_LED__
#define __GPIO_LED__

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f10x.h" 
extern unsigned char blink;
void Timer_Config(void);
void PWM_Config(void);
#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

