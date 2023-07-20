#include "watchdog.h"

void Watchdog_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	  /* Enable Watchdog*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	
	WWDG_DeInit();
	WWDG_SetPrescaler(WWDG_Prescaler_8); //1, 2, 4, 8
	WWDG_SetWindowValue(127); // 64...127
	WWDG_Enable(100);
	WWDG_EnableIT();

	
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    /*WWDG interrupt*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);/*    NVIC initialization*/
}

void WWDG_IRQHandler(void) {
	WWDG_ClearFlag(); //This function reset flag WWDG->SR and cancel the resetting
	WWDG_SetCounter(100);
}
