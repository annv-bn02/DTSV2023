#include "OTA.h"


uint8_t wait_success_frame, ota_state = 0;
int frame_tranfer_count = 0;
String str_ota_state;


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

uint8_t Check_Success_Frame(uint8_t success_frame_flag, uint8_t* buf)
{
    if(buf[0] != ETX_OTA_SOF)
    {
        return 0;
    }
    if(buf[1] != PACKET_TYPE_RESPONE)
    {
        return 0;
    }
    if(buf[2] != success_frame_flag)
    {
        return 0;
    }
    if(buf[3] != ETX_OTA_EOF)
    {
        return 0;
    }
    return 1;
}

void Ota_Respon_Frame()
{
  uint8_t buffer[100], bufferIndex = 0;
  while (Serial1.available()) {
    char incomingChar = Serial1.read(); // Đọc ký tự từ UART
    buffer[bufferIndex++] = incomingChar; // Lưu ký tự vào buffer và tăng chỉ số buffer
    Serial.print(incomingChar);
  }
  if(Check_Success_Frame(wait_success_frame, buffer))
  {
    wait_success_frame = 0;
  }
}

void Ota_Run(uint8_t *buf, uint32_t length) {
  unsigned int chia = length / 128;
  unsigned int index = 0;
  switch(ota_state)
  {
    case 0:
        str_ota_state = "Send Update Message";
        ota_state = 1;
        break;
    case 1:
        ota_send_update_message();
        Serial.print("\n<<<<<<ota_send_update_message>>>>>>\n");
        str_ota_state = "Send Start Frame";
        ota_state = 2;
        break;
    case 2:
        ota_send_start();
        Serial.print("\n<<<<<<ota_send_start>>>>>>\n");
        str_ota_state = "Send Header Frame";
        ota_state = 3;
        break;
    case 3:
        ota_send_header((chia + 1) * 128);
        Serial.print("\n<<<<<<ota_send_header>>>>>>\n");
        str_ota_state = "Send Data Frame";
        ota_state = 4;
        break;
    case 4:
        Serial.print("\n<<<<<<ota_send_data>>>>>>\n");
        if (chia * 128 == length) {
            unsigned int bufferLength = 128;
            uint8_t sampleBuffer[bufferLength];
            for (unsigned int i = 0; i < chia; i++) {
                for (unsigned int j = 0; j < 128; j++) {
                    sampleBuffer[j] = buf[index];
                    index++;
                }
                ota_send_data(sampleBuffer, 128);
                Serial.println("Frame :" + String(i));
                delay(100);
            }
        } else {
            unsigned int bufferLength = 128;
            uint8_t sampleBuffer[bufferLength];
            for (unsigned int i = 0; i < chia + 1; i++) {
                if (i < chia) {
                    for (unsigned int j = 0; j < 128; j++) {
                        sampleBuffer[j] = buf[index];
                        index++;
                    }
                } else {
                    unsigned int t = length - (chia * 128);
                    unsigned int count = 0;
                    for (unsigned int j = 0; j < 128; j++) {
                        if (count < t) {
                            sampleBuffer[j] = buf[index];
                        } else {
                            sampleBuffer[j] = 0xFF;
                        }
                        index++;
                        count++;
                    }
                }
                ota_send_data(sampleBuffer, 128);
                Serial.println("Frame :" + String(i));
                delay(100);
            }
        }
        ota_state = 5;
        str_ota_state = "Send End Frame";
        break;
    case 5:
        Serial.print("\n<<<<<<ota_send_end>>>>>>\n");
        ota_send_end();
        str_ota_state = "Success";
        success_flag = 1;
        ota_state = 0;
        flag_ota = 0;
        break;
  }
  delay(2000);
}

void ota_send_update_message() {
    unsigned char sof = 0xAA;
    unsigned char update_1 = 0x10;
    unsigned char update_2 = 0x11;
    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[4];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = update_1;
    sampleBuffer[2] = update_2;
    sampleBuffer[3] = eof;

    Serial2.write(sampleBuffer, 4);
}

void ota_send_start() {
    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x00;
    unsigned char data_len[2] = {0x00, 0x01};
    unsigned char cmd = 0x00;
    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[8];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    sampleBuffer[2] = data_len[1];
    sampleBuffer[3] = data_len[0];
    sampleBuffer[4] = cmd;

    // Calculate and set crc
    data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = CheckSum(sampleBuffer, 5);
    sampleBuffer[5] = input_convert.bytes[0];
    sampleBuffer[6] = input_convert.bytes[1];
    sampleBuffer[7] = eof;

    Serial2.write(sampleBuffer, 8);
}

void ota_send_end() {
    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x00;
    unsigned char data_len[2] = {0x00, 0x01};
    unsigned char cmd = 0x01;
    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[8];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    sampleBuffer[2] = data_len[1];
    sampleBuffer[3] = data_len[0];
    sampleBuffer[4] = cmd;

    // Calculate and set crc
    data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = CheckSum(sampleBuffer, 5);
    sampleBuffer[5] = input_convert.bytes[0];
    sampleBuffer[6] = input_convert.bytes[1];
    sampleBuffer[7] = eof;

    Serial2.write(sampleBuffer, 8);
}

void ota_send_header(int dataLength) {
    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x02;
    unsigned char data_len[2] = {0x00, 0x04};
    // unsigned char sizedata[4] = Convert_From_Int_To_Bytes(dataLength);
    // Convert dataLength to bytes and store in sizedata

    unsigned char eof = 0xBB;

    unsigned char sampleBuffer[11];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    sampleBuffer[2] = data_len[1];
    sampleBuffer[3] = data_len[0];
    data_format_int_bytes input_convert;
    input_convert.data_int = dataLength;
    sampleBuffer[4] = input_convert.bytes[0];
    sampleBuffer[5] = input_convert.bytes[1];
    sampleBuffer[6] = input_convert.bytes[2];
    sampleBuffer[7] = input_convert.bytes[3];
    // Calculate and set crc
    data_format_uint8_16_t uint16_convert;
    uint16_convert.data_uint16 = CheckSum(sampleBuffer, 8);
    sampleBuffer[8] = uint16_convert.bytes[0];
    sampleBuffer[9] = uint16_convert.bytes[1];
    sampleBuffer[10] = eof;

    Serial2.write(sampleBuffer, 11);
}

void ota_send_data(uint8_t *data, uint16_t length) {
    unsigned int index = 0;

    unsigned char sof = 0xAA;
    unsigned char packet_type = 0x01;
    // unsigned char data_len[2] = Convert_From_Uint16_To_Bytes(length);
    // Convert length to bytes and store in data_len

    unsigned char eof = 0xBB;

    unsigned int bufferLength = 7 + length;
    unsigned char sampleBuffer[bufferLength];
    sampleBuffer[0] = sof;
    sampleBuffer[1] = packet_type;
    data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = length;
    sampleBuffer[2] = input_convert.bytes[0];
    sampleBuffer[3] = input_convert.bytes[1];
    index = 4;

    for (unsigned int i = 0; i < length; i++) {
        sampleBuffer[index] = data[i];
        index++;
    }

    // Calculate and set crc
    input_convert;
    input_convert.data_uint16 = CheckSum(sampleBuffer, index);
    sampleBuffer[index] = input_convert.bytes[0];
    sampleBuffer[index + 1] = input_convert.bytes[1];
    sampleBuffer[index + 2] = eof;

    Serial2.write(sampleBuffer, index + 3);
}

uint16_t CheckSum(uint8_t *buf, uint8_t len)
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
