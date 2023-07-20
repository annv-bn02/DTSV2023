/*
  ******************************************************************************
  * @file		flash.h                                                             *
  * @author	    Nguyen Van An                                                *
  * @date		23/07/2022                                                      *
  ******************************************************************************
*/
	
#ifndef __FLASH__
#define __FLASH__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"                   // Device header
#include "stm32f10x_flash.h"            // Keil::Device:StdPeriph Drivers:Flash

#define WAIT_BUSY 	while((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY){}

#define FLASH_STT_BOOTLOADER_ADDRESS 0x0801C000	
	
	
extern uint8_t arr_data8bit[1000];
extern uint16_t arr_data16bit[1000];
extern uint32_t arr_data32bit[1000];
extern uint64_t arr_data64bit[1000];
void Unlock_Flash(void);
void Lock_Flash(void);
void Flash_PageErase(uint32_t vr_page_address);
void Flash_AllPageErase(void);
void Check_Busy_And_Size(void);

void  Flash_WriteStruct(uint32_t struct_address);
void  Flash_WriteArray8bit(uint32_t *vr_page_address,uint8_t* arr_data, uint16_t vr_lenght);
void  Flash_WriteArray16bit(uint32_t *vr_page_address,uint16_t* arr_data,  uint16_t vr_lenght);
void  Flash_WriteArray32bit(uint32_t *vr_page_address,uint32_t *arr_data,  uint16_t vr_lenght);
void  Flash_WriteArray64bit(uint32_t *vr_page_address,uint64_t *arr_data,  uint16_t vr_lenght);

void Flash_ReadArray8bit(uint32_t *vr_page_address, uint16_t vr_lenght);
void Flash_ReadArray16bit(uint32_t *vr_page_address, uint16_t vr_lenght);
void Flash_ReadArray32bit(uint32_t *vr_page_address, uint16_t vr_lenght);
void Flash_ReadArray64bit(uint32_t *vr_page_address, uint16_t vr_lenght);

void  Flash_Write8bit(uint32_t *vr_page_address,uint8_t arr_data);
void  Flash_Write16bit(uint32_t *vr_page_address,uint16_t arr_data);
void  Flash_Write32bit(uint32_t *vr_page_address,uint32_t arr_data);
void  Flash_Write64bit(uint32_t *vr_page_address,uint64_t arr_data);

uint8_t Flash_Read8bit(uint32_t *vr_page_address);
uint16_t Flash_Read16bit(uint32_t *vr_page_address);
uint32_t Flash_Read32bit(uint32_t *vr_page_address);
uint64_t Flash_Read64bit(uint32_t *vr_page_address);

void Flash_Update_Bootloader(void);
#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

