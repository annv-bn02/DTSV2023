#ifndef _OTA_
#define _OTA_

#include "sys.h"

#define ETX_OTA_SOF  0xAA    // Start of Frame
#define ETX_OTA_EOF  0xBB    // End of Frame
#define PACKET_TYPE_RESPONE 0x03 //packet type response frame
/*
 * Frame success
 */
typedef enum
{
	ETX_OTA_SUCCESS_SF = 0X01,    // OTA start frame success
	ETX_OTA_SUCCESS_HF = 0X02,    // OTA header frame success
	ETX_OTA_SUCCESS_DF = 0X03,    // OTA data  frame success
	ETX_OTA_SUCCESS_EF = 0X04,    // OTA end frame success
}ETX_OTA_SUCCESS_;

/**
 * @brief union convert int <-> uint8_t array
 * 
 */
typedef union 
{
    uint32_t data_int;
    uint8_t bytes[4];
} data_format_int_bytes;

/**
 * @brief union convert uint16_t <-> uint8_t array
 * 
 */
typedef union 
{
    uint16_t data_uint16;
    uint8_t bytes[2];
} data_format_uint8_16_t;

extern uint8_t wait_success_frame, flag_ota;
extern int frame_tranfer_count;
extern String str_ota_state;
uint8_t* Convert_From_Int_To_Bytes(int data);
uint8_t* Convert_From_Uint16_To_Bytes(int data);
uint8_t Check_Success_Frame(uint8_t success_frame_flag, uint8_t* buf);
void Ota_Respon_Frame();
void ota_send_update_message();
void ota_send_start();
void ota_send_end();
void ota_send_data(uint8_t *data, uint16_t length);
void ota_send_header(int dataLength);
void Ota_Run(uint8_t *buf, uint32_t length);
uint16_t CheckSum(uint8_t *buf, uint8_t len);

#endif