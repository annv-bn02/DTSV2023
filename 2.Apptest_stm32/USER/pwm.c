#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "pwm.h"                  // Device header
#include "debug.h"
unsigned int tim_prescaler=72;
unsigned int tim_period=1000;
unsigned char blink = 0;
void Timer_Config(void)
{
	TIM_TimeBaseInitTypeDef Timer_struct, pwm_struct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	pwm_struct.TIM_CounterMode		= TIM_CounterMode_Up;
	pwm_struct.TIM_Prescaler			= tim_prescaler - 1;
	pwm_struct.TIM_Period				= tim_period - 1;
	pwm_struct.TIM_ClockDivision		= 0;
	pwm_struct.TIM_RepetitionCounter	= 0;
	TIM_TimeBaseInit(TIM4, &pwm_struct);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	Timer_struct.TIM_CounterMode		= TIM_CounterMode_Up;
	Timer_struct.TIM_Prescaler			= 200 - 1;
	Timer_struct.TIM_Period				= 36000 - 1;
	Timer_struct.TIM_ClockDivision		= 0;
	Timer_struct.TIM_RepetitionCounter	= 0;
	TIM_TimeBaseInit(TIM2, &Timer_struct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)
	{		
		blink =! blink;
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void PWM_Config(void)
{
	TIM_OCInitTypeDef timer_oc;
	GPIO_InitTypeDef pin_pwm;
	timer_oc.TIM_OCMode 		= TIM_OCMode_PWM1;
	timer_oc.TIM_Pulse 			= 500;
	timer_oc.TIM_OCPolarity 	= TIM_OCPolarity_High;
	timer_oc.TIM_OutputState 	= TIM_OutputState_Enable;
	TIM_OC4Init(TIM4,&timer_oc);
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	pin_pwm.GPIO_Mode 	= GPIO_Mode_AF_PP;
	pin_pwm.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7;
	pin_pwm.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&pin_pwm);
}

