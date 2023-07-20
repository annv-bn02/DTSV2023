#include "flash.h"
#include "debug.h"

uint8_t arr_data8bit[1000];
uint16_t arr_data16bit[1000];
uint32_t arr_data32bit[1000];
uint64_t arr_data64bit[1000];
//////////////////////START WRITE ARRAY

void Flash_WriteArray8bit(uint32_t *vr_page_address,uint8_t *arr_data, uint16_t vr_lenght)
{   
	uint16_t i;
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	for(i=0; i < vr_lenght; i++)
	{  
		WAIT_BUSY
		*(uint16_t*)*vr_page_address = arr_data[i];
		*vr_page_address += 2U;
	}
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
		
}

void  Flash_WriteArray16bit(uint32_t *vr_page_address,uint16_t *arr_data, uint16_t vr_lenght)
{
	uint16_t i;
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	for(i=0; i < vr_lenght; i++)
	{  
		WAIT_BUSY
		*(uint16_t*)*vr_page_address = arr_data[i];
		*vr_page_address += 2U;
	}

	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}

void  Flash_WriteArray32bit(uint32_t *vr_page_address,uint32_t *arr_data, uint16_t vr_lenght)
{
	uint16_t i;
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	for(i=0; i < vr_lenght; i++)
	{  
		WAIT_BUSY
		*(uint16_t*)*vr_page_address = (uint16_t)(arr_data[i] & 0xffff);
		*vr_page_address += 2U;
		WAIT_BUSY
		*(uint16_t*)*vr_page_address = (uint16_t)(arr_data[i]>>16 & 0xffff);
		*vr_page_address += 2U;
	}
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}

void  Flash_WriteArray64bit(uint32_t *vr_page_address,uint64_t *arr_data, uint16_t vr_lenght)
{
	uint16_t i;
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	for(i=0; i < vr_lenght; i++)
	{  
		WAIT_BUSY
		*(uint16_t*)*vr_page_address = (uint16_t)(arr_data[i] & 0xffff);
		WAIT_BUSY
		*vr_page_address += 2U;
		*(uint16_t*)*vr_page_address = (uint16_t)(arr_data[i]>>16 & 0xffff);
		WAIT_BUSY
		*vr_page_address += 2U;
		*(uint16_t*)*vr_page_address = (uint16_t)(arr_data[i]>>32 & 0xffff);
		WAIT_BUSY
		*vr_page_address += 2U;
		*(uint16_t*)*vr_page_address = (uint16_t)(arr_data[i]>>48 & 0xffff);
		*vr_page_address += 2U;
	}
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}

//////////////////////END WRITE ARRAY

//////////////////////START READ ARRAY

void Flash_ReadArray8bit(uint32_t *vr_page_address,uint16_t vr_lenght)
{
	uint16_t i;
	for(i = 0; i < vr_lenght; i++)
	{
		arr_data8bit[i] = *(uint8_t *)*vr_page_address;
		*vr_page_address += 2U;
	}
}


void Flash_ReadArray16bit(uint32_t *vr_page_address, uint16_t vr_lenght)
{
	uint16_t i;
	for(i = 0; i < vr_lenght; i++)
	{
		arr_data16bit[i] = *(uint16_t *)*vr_page_address;
		*vr_page_address += 2U;
	}
}


void Flash_ReadArray32bit(uint32_t *vr_page_address,uint16_t vr_lenght)
{
	uint16_t i;
	for(i = 0; i < vr_lenght; i++)
	{
		arr_data32bit[i]  = *(uint32_t*)*vr_page_address;
		*vr_page_address += 4U;
	}
}

void Flash_ReadArray64bit( uint32_t *vr_page_address,uint16_t vr_lenght)
{
	uint16_t i;
	for(i = 0; i < vr_lenght; i++)
	{
		arr_data64bit[i] =*(uint64_t*)*vr_page_address;
		*vr_page_address += 8U;
	}

}

//////////////////////END WRITE ARRAY

//////////////////START WRITE SINGLE DATA

void  Flash_Write8bit(uint32_t *vr_page_address,uint8_t data)
{
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	WAIT_BUSY
	*(uint16_t*)*vr_page_address = data;
	*vr_page_address += 2U;
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}

void  Flash_Write16bit(uint32_t *vr_page_address,uint16_t data)
{
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	WAIT_BUSY
	*(uint16_t*)*vr_page_address = data;
	*vr_page_address += 2U;
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}

void  Flash_Write32bit(uint32_t *vr_page_address,uint32_t data)
{
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	WAIT_BUSY
	*(uint16_t*)*vr_page_address = (uint16_t)(data & 0xffff);
	*vr_page_address += 2U;
	WAIT_BUSY
	*(uint16_t*)*vr_page_address = (uint16_t)(data>>16 & 0xffff);
	*vr_page_address += 2U;
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}
void  Flash_Write64bit(uint32_t *vr_page_address, uint64_t data)
{
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PG;
	WAIT_BUSY
	*(uint16_t*)*vr_page_address = (uint16_t)(data & 0xffff);
	WAIT_BUSY
	*vr_page_address += 2U;
	*(uint16_t*)*vr_page_address = (uint16_t)(data>>16 & 0xffff);
	WAIT_BUSY
	*vr_page_address += 2U;
	*(uint16_t*)*vr_page_address = (uint16_t)(data>>32 & 0xffff);
	WAIT_BUSY
	*vr_page_address += 2U;
	*(uint16_t*)*vr_page_address = (uint16_t)(data>>48 & 0xffff);
	*vr_page_address += 2U;
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PG;	
}

//////////////////END WRITE SINGLE DATA

//////////////////START READ SINGLE DATA

uint8_t Flash_Read8bit(uint32_t *vr_page_address)
{
	uint16_t tmp = *(uint32_t *)*vr_page_address;
	*vr_page_address += 2U;
	return tmp;
}

uint16_t Flash_Read16bit(uint32_t *vr_page_address)
{
	uint16_t tmp = *(uint32_t *)*vr_page_address;
	*vr_page_address += 2U;
	return tmp;
}

uint32_t Flash_Read32bit(uint32_t *vr_page_address)
{
	uint32_t tmp = *(uint32_t *)*vr_page_address;
	*vr_page_address += 4U;
	return tmp;
}

uint64_t Flash_Read64bit( uint32_t *vr_page_address)
{
	uint64_t tmp = *(uint64_t *)*vr_page_address;
	*vr_page_address += 8U;
	return tmp;
}

//////////////////END READ SINGLE DATA

void Lock_Flash(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void Unlock_Flash(void)
{
	FLASH->KEYR =  0x45670123;
	FLASH->KEYR =  0xCDEF89AB;
}

void Flash_PageErase(uint32_t vr_page_address)
{
	//wait busy reset
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = vr_page_address;
	FLASH->CR |= FLASH_CR_STRT;
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_PER;
	FLASH->CR &= ~FLASH_CR_STRT;
}

void Flash_AllPageErase(void)
{
	//wait busy reset
	WAIT_BUSY
	FLASH->CR |= FLASH_CR_MER;
	FLASH->CR |= FLASH_CR_STRT;
	WAIT_BUSY
	FLASH->CR &= ~FLASH_CR_MER;
	FLASH->CR &= ~FLASH_CR_STRT;
}

void Flash_Update_Bootloader(void)
{
	uint32_t stt_booloader_address = FLASH_STT_BOOTLOADER_ADDRESS;
	db_DEBUG_Putchar("Update");
	Flash_PageErase(stt_booloader_address);
	Flash_Write8bit(&stt_booloader_address, 1);
	NVIC_SystemReset();
}
