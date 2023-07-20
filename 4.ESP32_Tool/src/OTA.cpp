#include "OTA.h"



uint8_t *Convert_From_Int_To_Bytes(int data)
{
    static data_format_int_bytes input_convert;
    input_convert.data_int = data;
    return input_convert.bytes;
}

uint8_t *Convert_From_Uint16_To_Bytes(uint16_t data)
{
    static data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = data;
    return input_convert.bytes;
}
