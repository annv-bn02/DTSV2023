/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <flash.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

unsigned char code_save[1024];
unsigned char rx_data,rx_buff[30];
uint32_t count=0,size_program=0,size_count=0;
uint8_t flag=0;
int page_write=8;
int end=0;
char goto_aplication=0;

#define PROGRAM_START_PAGE 8
#define PROGRAM_START_ADDRESS 		 	((uint32_t)0x08000000 + ((uint32_t)PROGRAM_START_PAGE*1024))	//Page 8

void Erase_PAGE(uint32_t addr)
{
Flash_Unlock();
  while((FLASH->SR&FLASH_SR_BSY));
  FLASH->CR |= FLASH_CR_PER; //Page Erase Set
  FLASH->AR = addr; //Page Address
  FLASH->CR |= FLASH_CR_STRT; //Start Page Erase
  while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR &= ~FLASH_SR_BSY;
  FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear
Flash_Lock();
}
uint16_t F_read(uint32_t addr)
{
	uint16_t* val = (uint16_t *)addr;
	return *val;
}
char Write_PAGE(uint32_t size,uint32_t page)
{
	uint32_t ADDRESS_PAGE = ((uint32_t)0x08000000 + (page*1024)); //tinh dia chi cua page
	uint32_t addr;
	uint16_t data;
	Flash_Erase(ADDRESS_PAGE);
	Flash_Unlock();
	for(int i=0;i<size/2;i++)
	{
		addr = ADDRESS_PAGE + (i*2);
		FLASH->CR |= FLASH_CR_PG;				/*!< Programming */
		while((FLASH->SR&FLASH_SR_BSY));
		data=((uint16_t)code_save[i*2+1]<<8) | ((uint16_t)code_save[i*2]);
		*(__IO uint16_t*)addr = data;
		while((FLASH->SR&FLASH_SR_BSY));
		FLASH->CR &= ~FLASH_CR_PG;
	}
	//check lại
	for(int i=0;i<size/2;i++)
	{
		addr = ADDRESS_PAGE + (i*2);
		data=((uint16_t)code_save[i*2+1]<<8) | ((uint16_t)code_save[i*2]);
		uint16_t test2=F_read(addr);
		if(data != test2)
			{
				Flash_Lock();
				return 1;
			}
	}
	Flash_Lock();
	return 0 ;
}

void clear_buffer()
{
	for(int i=0;i<1024;i++)code_save[i]=255;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(flag==0)
	{
		 rx_buff[count++]=rx_data;
		 if(rx_data == '\n')
			 {				 
				 flag=1;				 
				 int ts=1;
				 size_program=0;size_count=0;end=0;
				 page_write=PROGRAM_START_PAGE;
				 for(count=count-2;count>0;count--)
				 {
					 if(rx_buff[count] == '=')break;
					 size_program+= (rx_buff[count]-48)*ts;
					 ts*=10;
				 }
				 if(size_program>1024)HAL_UART_Receive_IT(&huart2,code_save,1024);
				 else HAL_UART_Receive_IT(&huart2,code_save,size_program);
				 HAL_UART_Transmit(&huart2,(uint8_t *)"OK MEN !\r\n",10,100);
			 }
		 else
			 {
				 HAL_UART_Receive_IT(&huart2,(uint8_t *)&rx_data,1);
			 }
	}
	else if(flag==1)
	{	
		if(end==1)
		{			
			HAL_UART_Receive_IT(&huart2,(uint8_t *)&rx_data,1);
			if(Write_PAGE(1024,page_write) == 0)HAL_UART_Transmit(&huart2,(uint8_t *)"Read OK !\r\n",11,100);
			else 
				{
					 flag=0;
					 HAL_UART_Transmit(&huart2,(uint8_t *)"Write Fall !\r\n",14,100);
					 HAL_UART_Receive_IT(&huart2,(uint8_t *)&rx_data,1);
				}
			clear_buffer();		
			flag=0;
			goto_aplication=1; //chuyen toi ct BOOT		
			return;
		}
		size_count+=1024;
		if((size_count+1024) < size_program)
			HAL_UART_Receive_IT(&huart2,code_save,1024);
		else 		 
		{
			HAL_UART_Receive_IT(&huart2,code_save,size_program-size_count);
			end=1;
		}
		if(Write_PAGE(1024,page_write) == 0)HAL_UART_Transmit(&huart2,(uint8_t *)"Read OK !\r\n",11,100);
		else 
				{
					 flag=0;
					 HAL_UART_Transmit(&huart2,(uint8_t *)"Write Fall !\r\n",14,100);
					 HAL_UART_Receive_IT(&huart2,(uint8_t *)&rx_data,1);
				}
		page_write++;	
		clear_buffer();
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
	HAL_UART_Receive_IT(&huart2,(uint8_t *)&rx_data,1);
  /* USER CODE BEGIN 2 */
	clear_buffer();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(goto_aplication==1)
		{
			void (*SysMemBootJump)(void);
			volatile uint32_t addr = PROGRAM_START_ADDRESS;
			#if defined(USE_HAL_DRIVER)
      HAL_RCC_DeInit();
			#endif /* defined(USE_HAL_DRIVER) */
			#if defined(USE_STDPERIPH_DRIVER)
					RCC_DeInit();
			#endif /* defined(USE_STDPERIPH_DRIVER) */
			
			SysTick->CTRL = 0;
      SysTick->LOAD = 0;
      SysTick->VAL = 0;
			
			__disable_irq();
			
			#if defined(STM32F4)
      SYSCFG->MEMRMP = 0x01;
			#endif
			#if defined(STM32F0)
					SYSCFG->CFGR1 = 0x01;
			#endif
			
			SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));
			
			__set_MSP(*(uint32_t *)addr);

			SysMemBootJump();
			
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
