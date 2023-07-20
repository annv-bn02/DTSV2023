#ifndef _OTA_
#define _OTA_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdlib.h"
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

uint8_t* Convert_From_Int_To_Bytes(int data);
uint8_t* Convert_From_Uint16_To_Bytes(int data);
#ifdef __cplusplus
}
#endif

#endif