#include "ota_uart_fram.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "debug.h"
#include "flash_if.h"
#include "usart.h"
ETX_OTA_VR_t OTA_Data;
static ETX_OTA_FRAME_DATA_	ota_frame = ETX_OTA_FRAME_START;
static ETX_OTA_STATE_ 		ota_state = ETX_OTA_STATE_START;

uint8_t ArrTemp[128] = {0};
uint8_t checkpoint = 0;
uint16_t crc, crc_byte, crc_receive, se_flag;
uint32_t datasize;
uint32_t SizeFileBin;
uint32_t Count_SizeFileBin = FLASH_APP_TEMP_ADDR;
uint32_t tempss = 0;

static void ETX_Init_OTAdata(void);
static void ETX_Reset_Index(void);
static void ETX_End_Frame(void);
static void ETX_Process_Frame(void);
static void ETX_Program_Main_App(void);

void ETX_Run(void)
{
	ETX_Process_Frame();
}

static void ETX_Process_Frame(void)
{
	if(OTA_Data.FlagFrame == 1)
	{
		OTA_Data.FlagFrame = 0;
		switch( ota_state )
		{
		  case ETX_OTA_STATE_START:
		  {
			ETX_OTA_COMMAND_t *cmd = (ETX_OTA_COMMAND_t*)OTA_Data.Buffer;
			if( cmd->packet_type == ETX_OTA_PACKET_TYPE_CMD )
			{
			  if( cmd->cmd == ETX_OTA_CMD_START )
			  {
					FLASH_If_Erase(FLASH_APP_TEMP_ADDR);
					ota_state = ETX_OTA_STATE_HEADER;
					checkpoint = 1;
			  }
			}
			else
			{
				Count_SizeFileBin = FLASH_APP_TEMP_ADDR;
				tempss = 0;
				SizeFileBin = 0;
			}
		  }
		  break;

		  case ETX_OTA_STATE_HEADER:
			{
				u8Tou32_u TempData;
				ETX_OTA_HEADER_t *header = (ETX_OTA_HEADER_t*)OTA_Data.Buffer;
				if( header->packet_type == ETX_OTA_PACKET_TYPE_HEADER )
				{
					ota_state = ETX_OTA_STATE_DATA;
					TempData.Data_In[0] = OTA_Data.Buffer[4];
					TempData.Data_In[1] = OTA_Data.Buffer[5];
					TempData.Data_In[2] = OTA_Data.Buffer[6];
					TempData.Data_In[3] = OTA_Data.Buffer[7];
					SizeFileBin = TempData.Data_Out;
					checkpoint = 3;
				}
				else
				{
					SizeFileBin = 0;
					ota_state = ETX_OTA_STATE_START;
				}
			}
		  break;

		  case ETX_OTA_STATE_DATA:
			{
				uint8_t i;
				u8Tou16_u datatemp;
				uint32_t *dd;
				checkpoint = 3;
				if(OTA_Data.Buffer[1]== ETX_OTA_PACKET_TYPE_DATA)
				{
					datatemp.Data_In[0] = OTA_Data.Buffer[2];
					datatemp.Data_In[1] = OTA_Data.Buffer[3];
					datasize = datatemp.Data_Out;
					
					for(i=0; i<datasize; i++)
					{
						ArrTemp[i] = OTA_Data.Buffer[i+4];
					}
					dd = (uint32_t*)ArrTemp;
					FLASH_If_Write(&Count_SizeFileBin, (uint32_t*)dd, (uint32_t)datasize/4);	
					tempss = Count_SizeFileBin - FLASH_APP_TEMP_ADDR;
					if(tempss >= SizeFileBin)
					{
						ota_state = ETX_OTA_STATE_END;
						checkpoint = 4;
					}
				}
			
			}
		  break;

		  case ETX_OTA_STATE_END:
			{
				ETX_OTA_COMMAND_t *cmd = (ETX_OTA_COMMAND_t*)OTA_Data.Buffer;
				if( cmd->packet_type == ETX_OTA_PACKET_TYPE_CMD )
				{
				  if( cmd->cmd == ETX_OTA_CMD_END )
				  {
						checkpoint = 5;
						ota_state = ETX_OTA_STATE_START;
						ETX_Program_Main_App();
						FLASH_If_Finish();
				  }
				}
				else
				{
					checkpoint = 6;
					ota_state = ETX_OTA_STATE_START;
					FLASH_If_Finish();
				}
			}
		  break;
			}
		}
}

void ETX_Receive_Chunk(void)
{
	FLASH_If_Init();
	OTA_Data.Buffer[0] =0;
	ota_state = ETX_OTA_STATE_START;
	ETX_Init_OTAdata();
} 

static void ETX_Reset_Index(void)
{
	OTA_Data.Index = 0;
	OTA_Data.Length = 0;
	OTA_Data.FlagFrame = 0;
	ota_frame= ETX_OTA_FRAME_START;
}

static void ETX_End_Frame(void)
{
	ota_frame= ETX_OTA_FRAME_START;
}

static void ETX_Init_OTAdata(void)
{
	OTA_Data.Index = 0;
	OTA_Data.Length = 0;
	OTA_Data.FlagFrame = 0;
	memset( OTA_Data.Buffer, 0, ETX_OTA_PACKET_MAX_SIZE );
}

static void ETX_Program_Main_App(void)
{
	int i;
	uint32_t address_main = FLASH_APP_MAIN_ADDR, address_tmp = FLASH_APP_TEMP_ADDR, stt_booloader_address = FLASH_STT_BOOTLOADER_ADDRESS;;
	FLASH_If_Erase(FLASH_APP_MAIN_ADDR);
	FLASH->KEYR =  0x45670123;
	FLASH->KEYR =  0xCDEF89AB;
	for(i = 0; i < SizeFileBin/4; i++)
	{
		Flash_Write32bit(&address_main, Flash_Read32bit(&address_tmp));
	}
	Flash_PageErase(stt_booloader_address);
	Flash_Write8bit(&stt_booloader_address, 0);
	NVIC_SystemReset();
}

void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		uint8_t temp_char = USART_ReceiveData(USART1);
		switch(ota_frame)
		{
			case ETX_OTA_FRAME_START:
				if(temp_char != ETX_OTA_SOF)
				{
					ETX_Init_OTAdata();
					USART_SendString("Start Frame ERROR\n");
				}
				else
				{
					ETX_Init_OTAdata();
					ota_frame = ETX_OTA_FRAME_TYPE;
					OTA_Data.Buffer[OTA_Data.Index] = temp_char;
					OTA_Data.Index++;
				}
			break;
			
			case ETX_OTA_FRAME_TYPE:
				ota_frame = ETX_OTA_FRAME_LENGTH;
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				if(temp_char == 0x00)
				{
					se_flag = 1;
				}
				else
				{
					se_flag = 0;
				}
				OTA_Data.Index++;
			break;
					
			case ETX_OTA_FRAME_LENGTH:
				if(OTA_Data.Index >= 4)
				{
					u8Tou16_u Tempdata;
					ota_frame = ETX_OTA_FRAME_DATA;
					Tempdata.Data_In[0] = OTA_Data.Buffer[2];
					Tempdata.Data_In[1] = OTA_Data.Buffer[3];
					OTA_Data.Length = Tempdata.Data_Out;
				}
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
			break;
							
			case ETX_OTA_FRAME_DATA:
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
				if((OTA_Data.Index - 4) >= (OTA_Data.Length))
				{
					ota_frame = ETX_OTA_FRAME_CRC;
					crc_byte = 0;
					crc = Check_Sum(OTA_Data.Buffer, OTA_Data.Index - se_flag);
				}
			break;
									
			case ETX_OTA_FRAME_CRC:
				OTA_Data.Buffer[OTA_Data.Index] = temp_char;
				OTA_Data.Index++;
				if((OTA_Data.Index - 2 - (OTA_Data.Length)) >= (4)) //2 - CHECKSUM LENGTH
				{
					u8Tou16_u Tempdata;
					Tempdata.Data_In[1] = OTA_Data.Buffer[OTA_Data.Index - 1];
					Tempdata.Data_In[0] = OTA_Data.Buffer[OTA_Data.Index - 2];
					crc_receive = Tempdata.Data_Out;
					if(Tempdata.Data_Out == crc)
					{
						ota_frame = ETX_OTA_FRAME_END;	
					}
					else
					{
						USART_SendString("Check Sum ERROR\n");
						NVIC_SystemReset();			
					}
				}
			break;
											
			case ETX_OTA_FRAME_END:
				if(temp_char != ETX_OTA_EOF)
				{
					ETX_Reset_Index();
					USART_SendString("End Frame ERROR\n");
				}
				else
				{
					OTA_Data.FlagFrame = 1;
					OTA_Data.Buffer[OTA_Data.Index] = temp_char;
					OTA_Data.Index++;
					OTA_Data.Buffer[OTA_Data.Index] = 0x00;
					ETX_End_Frame();
				}
			break;
		}
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

uint16_t Check_Sum(uint8_t *buf, uint8_t len)
{
	uint16_t crc = 0xFFFF, pos = 0, i = 0;
	for (pos = 0; pos < len; pos++)
	{
		crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
		for (i = 8; i != 0; i--)   // Loop over each bit
		{
			if ((crc & 0x0001) != 0) // If the LSB is set
			{
				crc >>= 1; // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else // Else LSB is not set
			{
				crc >>= 1; // Just shift right
			}
		}
	}
	return crc;
}
