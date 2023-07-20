

#ifndef _DELAY_
#define _DELAY_


#ifdef __cplusplus
extern "C"{
#endif
	
void Fn_DELAY_Short (unsigned int _vrui_Time);
void Fn_DELAY_Long  (unsigned int _vrui_Time);

void Fn_DELAY_Init (unsigned char _CLK);
void Fn_Delay_Us (unsigned long _vrui_Time);
void Fn_Delay_Ms (unsigned int _vrui_Time);

#ifdef __cplusplus
}
#endif

#endif


