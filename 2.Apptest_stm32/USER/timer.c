#include "timer.h"
unsigned char blink = 0, blink_flag = 1;
unsigned int blink_cnt, delay_ms_cnt = 0, delay_us_cnt = 0;
void Timer_Config()
{
	TIM_TimeBaseInitTypeDef timer_struct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timer_struct.TIM_CounterMode		= TIM_CounterMode_Up;
	timer_struct.TIM_Prescaler			= 2 - 1;
	timer_struct.TIM_Period				= 36000 - 1;
	timer_struct.TIM_ClockDivision		= 0;
	timer_struct.TIM_RepetitionCounter	= 0;
	TIM_TimeBaseInit(TIM2, &timer_struct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)
	{		
//		blink =! blink;
//		blink_flag = 1;
		if(blink_cnt++ == 1000)
		{
			blink_cnt = 0;
			blink =! blink;
			blink_flag = 1;
		}
//		if(delay_ms_cnt)
//		{
//			delay_ms_cnt--;
//		}
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

//void Delay_Ms(unsigned int time_ms){
//	
//	delay_ms_cnt = time_ms;
//	while(delay_ms_cnt);
//}
//void Delay_Us(unsigned int time_us){
//	delay_us_cnt = time_us;
//	while(delay_us_cnt);
//}
