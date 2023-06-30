#include "usart.h"
#include "string.h"
GPIO_TypeDef* GPIOx;
USART_TypeDef* USARTx = USART2;
char string_receive[1000];
char numberal[100][100];
char vr_GetChar;
int vr_locate = 0;
uint16_t count=0;
uint16_t flag_receive_done=0;
uint16_t Tx_pin,Rx_pin;
//struct __FILE 
//{
//	int dummy;
//};
//FILE __stdout;
//int fputc(int ch, FILE *f) 
//{
//	USART_SendChar(ch);
//	return ch;
//}

uint8_t USART_FlagDB(void)
{
	if(flag_receive_done==1)
	{
		flag_receive_done=0;
		return 1;
	}
	else 
	{
		return 0;
	}
}


uint8_t USART_Compare(char* string)
{
	uint16_t position = 0;
	while(string_receive[position] != '\0' || string[position] != '\0')
	{
		if(string_receive[position] != string[position])
		{
			return 0;
		}			
		position++;
	}		
	return 1;
}

uint16_t Numberal(int num, int vr_locate)
{
	uint16_t number_of_digits = 1;
	while(num > 0)
	{
		numberal[vr_locate][number_of_digits++] = num % 10;
		num /= 10;
	}
	return number_of_digits;
}

void USART_SendNum(int num) 
{
	int number_of_digits;
	number_of_digits = Numberal(num,vr_locate) - 1;
	while(number_of_digits > 0)
	{
		USART_SendChar(numberal[vr_locate][number_of_digits] + '0');
		number_of_digits--;
	}
}

void USART_Config(uint16_t Baurates)
{
	
    GPIO_InitTypeDef GPIO_Struct;
	USART_InitTypeDef USART_Struct;
	if(USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
		Tx_pin = GPIO_Pin_9;
		Rx_pin = GPIO_Pin_10;
		GPIOx = GPIOA;
	}
	else if(USARTx == USART2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		Tx_pin = GPIO_Pin_2;
		Rx_pin = GPIO_Pin_3;
		GPIOx = GPIOA;
	}
	else if(USARTx == USART3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		Tx_pin = GPIO_Pin_10;
		Rx_pin = GPIO_Pin_11;
		GPIOx = GPIOB;
	}
	
	GPIO_Struct.GPIO_Mode	 = GPIO_Mode_AF_PP;
	GPIO_Struct.GPIO_Pin     = Tx_pin;
	GPIO_Struct.GPIO_Speed	 = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_Struct);
	
	GPIO_Struct.GPIO_Mode	 = GPIO_Mode_IN_FLOATING;
	GPIO_Struct.GPIO_Pin     = Rx_pin;
	GPIO_Struct.GPIO_Speed	 = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_Struct);
	
	USART_Struct.USART_BaudRate 			= Baurates;
	USART_Struct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Struct.USART_Mode 				= USART_Mode_Rx|USART_Mode_Tx;
	USART_Struct.USART_Parity      			= USART_Parity_No;
	USART_Struct.USART_StopBits 			= USART_StopBits_1;
	USART_Struct.USART_WordLength 			= USART_WordLength_8b;
	USART_Init(USARTx,&USART_Struct);
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART3_IRQn);
	USART_Cmd(USARTx,ENABLE);
}

void USART_SendChar(char vr_Char)
{
	USART_SendData(USARTx,vr_Char);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}

void USART_SendString(char *vr_Str)
{
	while(*vr_Str != 0)
	{
		USART_SendChar(*vr_Str++);
	}
}

//void USART1_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		vr_GetChar = USART_ReceiveData(USART1);
//		USART_SendChar(vr_GetChar);
//		if(vr_GetChar != '\n')
//		{
//			string_receive[count] = vr_GetChar;
//			count++;
//		}
//		else
//		{
//			flag_receive_done=1;
//			string_receive[count - 1] = 0;
//			count = 0;
//		}
//	}
//}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		vr_GetChar = USART_ReceiveData(USART2);
		if(vr_GetChar != '\n')
		{
			string_receive[count] = vr_GetChar;
			count++;
		}
		else
		{
			flag_receive_done=1;
			string_receive[count-1] = 0;
			count = 0;
		}
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		vr_GetChar = USART_ReceiveData(USART3);
		if(vr_GetChar != '\n')
		{
			string_receive[count] = vr_GetChar;
			count++;
		}
		else
		{
			flag_receive_done=1;
			string_receive[count-1] = 0;
			count = 0;
		}
	}
}
