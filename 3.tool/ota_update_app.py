
from turtle import delay
import serial
import time
import os
import numpy
import struct


arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.8)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data

def send_ota_start():
    sof         = 0xAA
    packet_type = 0x00
    data_len    = struct.pack('H',1)
    cmd         = 0x00
    # crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 8
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0] = sof
    sampleBuffer[1] = packet_type
    sampleBuffer[2] = data_len[0]
    sampleBuffer[3] = data_len[1]
    sampleBuffer[4] = cmd
    crc = struct.pack('I',check_sum(sampleBuffer, 5));    
    sampleBuffer[5] = crc[0]
    sampleBuffer[6] = crc[1]
    sampleBuffer[7] = eof
    arduino.write(sampleBuffer)

def send_ota_end():
    sof         = 0xAA
    packet_type = 0x00
    data_len    = struct.pack('H',1)
    cmd         = 0x01
    # crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 8
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0] = sof
    sampleBuffer[1] = packet_type
    sampleBuffer[2] = data_len[0]
    sampleBuffer[3] = data_len[1]
    sampleBuffer[4] = cmd
    crc = struct.pack('I',check_sum(sampleBuffer, 5));    
    sampleBuffer[5] = crc[0]
    sampleBuffer[6] = crc[1]
    sampleBuffer[7] = eof
    arduino.write(sampleBuffer)

def send_ota_header(dataLength):
    sof         = 0xAA
    packet_type = 0x02
    data_len    = struct.pack('H',4)
    sizedata    = struct.pack('I',dataLength)
    # crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 11
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0]  = sof
    sampleBuffer[1]  = packet_type
    sampleBuffer[2]  = data_len[0]
    sampleBuffer[3]  = data_len[1]
    sampleBuffer[4]  = sizedata[0]
    sampleBuffer[5]  = sizedata[1]
    sampleBuffer[6]  = sizedata[2]
    sampleBuffer[7]  = sizedata[3]
    crc = struct.pack('I',check_sum(sampleBuffer, 8));    
    sampleBuffer[8]  = crc[0]
    sampleBuffer[9]  = crc[1]
    sampleBuffer[10] = eof
    arduino.write(sampleBuffer)

def send_ota_data(dataLength):
    length = len(dataLength)
    index = 0
    
    sof         = 0xAA
    packet_type = 0x01
    data_len    = struct.pack('H',length)
    #crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 7+length
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0]  = sof
    sampleBuffer[1]  = packet_type
    sampleBuffer[2]  = data_len[0]
    sampleBuffer[3]  = data_len[1]
    index = index + 4
    for i in range(length):
        sampleBuffer[index] = dataLength[i]
        index = index + 1
    crc = struct.pack('I',check_sum(sampleBuffer, index));    
    sampleBuffer[index]   = crc[0]
    sampleBuffer[index+1] = crc[1]
    sampleBuffer[index+2] = eof
    arduino.write(sampleBuffer)


def check_sum(buf, length):
    crc = 0xFFFF
    for pos in range(length):
        crc ^= buf[pos]  # XOR byte into least sig. byte of crc
        for i in range(8, 0, -1):  # Loop over each bit
            if crc & 0x0001:  # If the LSB is set
                crc >>= 1  # Shift right and XOR 0xA001
                crc ^= 0xA001
            else:  # Else LSB is not set
                crc >>= 1  # Just shift right
    return crc

def calculate_fram_data():
    # get the size of file
    path = r"F:\MCU\STM32F103\1.Code\13.Bootloader\2.Apptest_stm32\MDK\Objects\gpio_blynk.bin"
    size = os.path.getsize(path) 
    print('Size of file is', size, 'bytes')
    file = open(path, "rb")
    number = list(file.read(size))
    bytes_of_values = bytes(number)
    file.close()
    chia = size/128
    index = 0
    print("-------Start-------")  
    send_ota_start()
    time.sleep(2.5)
    print("-------Header-------")  
    send_ota_header((int(chia+1))*128)
    time.sleep(2.5)
    if (int(chia)*128) == size:
        bufferLength = 128
        sampleBuffer = bytearray(bufferLength)
        for i in range(int(chia)):
            print("-------LOADA: ", i,  "-------")  
            for j in range(128):
                    sampleBuffer[j] = number[index]
                    index = index + 1
            send_ota_data(sampleBuffer)
            time.sleep(0.1)
    else:
        bufferLength = 128
        sampleBuffer = bytearray(bufferLength)
        for i in range(int(chia)+1):
            print("-------LOAD: ", i,  "-------")  
            if(i < int(chia)):
                for j in range(128):
                    sampleBuffer[j] = number[index]
                    index = index + 1
            else:
                t = size - (int(chia)*128)
                count = 0
                for j in range(128):
                    if(count < t):
                        sampleBuffer[j] = number[index]
                    else:
                        sampleBuffer[j] = 0xff
                    index = index + 1
                    count = count + 1
            send_ota_data(sampleBuffer)
            time.sleep(0.1)
    time.sleep(2.5)
    send_ota_end()
    print("-------Done-------")                  
calculate_fram_data()