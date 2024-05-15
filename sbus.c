#include "sbus.h"
#include "usart.h"

#define SPEED_FAST      1696
#define SPEED_MID       1024
#define SPEED_SLOW      352
#define CHASSIS_FREE    352
#define CHASSIS_SLAVE   1696
#define USED_HUART      huart1 //change this to your used uart port

uint8_t sbus_buf[25];       //[start 0x0f] [data1] [data2] ... [data22] [flags] [end 0x00]
volatile uint16_t CH[17];   //[reserve] [CH1] [CH2] ... [CH16] // You can save the Number of enabled channels in CH[0]

void Sbus_Init(void)
{
    CH[ 1] = 1024;          //left(352) - 1024 - right(1696)
    CH[ 2] = 1024;          //back(352) - 1024 - forth(1696)
    CH[ 3] = 1024;          //[head pitch] down(352) - 1024 - up(1696)  (only in slave model)
    CH[ 4] = 1024;          //left(352) - 1024 - right(1696) //[under slave model]->[head course] || [under free model]->[chassis course]
    CH[ 5] = SPEED_MID;     //[speed] slow=352 mid=1024 fast=1696
    CH[ 6] = CHASSIS_FREE;  //CHASSIS_SLAVE CHASSIS_FREE ;//[chassis modle] free=352 slave=1696
    CH[ 7] = 1696;          //[chassis output] off=352 on=1696
    CH[ 8] = 0;
    CH[ 9] = 0;
    CH[10] = 0;
    CH[11] = 0;
    CH[12] = 0;
    CH[13] = 0;
    CH[14] = 0;
    CH[15] = 0;
    CH[16] = 0;
    Sbus_update();
}

//for receiving
void Sbus_Data_Decoding(void) 
{
    CH[ 1] = ((uint16_t)sbus_buf[ 1] >> 0 | ((uint16_t)sbus_buf[ 2] << 8 )) & 0x07FF;
    CH[ 2] = ((uint16_t)sbus_buf[ 2] >> 3 | ((uint16_t)sbus_buf[ 3] << 5 )) & 0x07FF;
    CH[ 3] = ((uint16_t)sbus_buf[ 3] >> 6 | ((uint16_t)sbus_buf[ 4] << 2 )  | (int16_t)sbus_buf[ 5] << 10 ) & 0x07FF;
    CH[ 4] = ((uint16_t)sbus_buf[ 5] >> 1 | ((uint16_t)sbus_buf[ 6] << 7 )) & 0x07FF;
    CH[ 5] = ((uint16_t)sbus_buf[ 6] >> 4 | ((uint16_t)sbus_buf[ 7] << 4 )) & 0x07FF;
    CH[ 6] = ((uint16_t)sbus_buf[ 7] >> 7 | ((uint16_t)sbus_buf[ 8] << 1 )  | (int16_t)sbus_buf[ 9] <<  9 ) & 0x07FF;
    CH[ 7] = ((uint16_t)sbus_buf[ 9] >> 2 | ((uint16_t)sbus_buf[10] << 6 )) & 0x07FF;
    CH[ 8] = ((uint16_t)sbus_buf[10] >> 5 | ((uint16_t)sbus_buf[11] << 3 )) & 0x07FF;

    CH[ 9] = ((uint16_t)sbus_buf[12] << 0 | ((uint16_t)sbus_buf[13] << 8 )) & 0x07FF;
    CH[10] = ((uint16_t)sbus_buf[13] >> 3 | ((uint16_t)sbus_buf[14] << 5 )) & 0x07FF;
    CH[11] = ((uint16_t)sbus_buf[14] >> 6 | ((uint16_t)sbus_buf[15] << 2 )  |  (int16_t)sbus_buf[16] << 10 ) & 0x07FF;
    CH[12] = ((uint16_t)sbus_buf[16] >> 1 | ((uint16_t)sbus_buf[17] << 7 )) & 0x07FF;
    CH[13] = ((uint16_t)sbus_buf[17] >> 4 | ((uint16_t)sbus_buf[18] << 4 )) & 0x07FF;
    CH[14] = ((uint16_t)sbus_buf[18] >> 7 | ((uint16_t)sbus_buf[19] << 1 )  |  (int16_t)sbus_buf[20] <<  9 ) & 0x07FF;
    CH[15] = ((uint16_t)sbus_buf[20] >> 2 | ((uint16_t)sbus_buf[21] << 6 )) & 0x07FF;
    CH[16] = ((uint16_t)sbus_buf[21] >> 5 | ((uint16_t)sbus_buf[22] << 3 )) & 0x07FF;
}


//for sending
void Sbus_update(void)
{
    sbus_buf[ 0] = 0x0F;							// [startbyte]
    sbus_buf[ 1] = (uint8_t)(CH[ 1] & 0xFF);		// [Data 1]
    sbus_buf[ 2] = (uint8_t)(CH[ 1] >> 8 | CH[ 2] << 3);
    sbus_buf[ 3] = (uint8_t)(CH[ 2] >> 5 | CH[ 3] << 6);
    sbus_buf[ 4] = (uint8_t)(CH[ 3] >> 2);
    sbus_buf[ 5] = (uint8_t)(CH[ 3] >> 10 | CH[4] <<1);
    sbus_buf[ 6] = (uint8_t)(CH[ 4] >> 7 | CH [5]<< 4);
    sbus_buf[ 7] = (uint8_t)(CH[ 5] >> 4 | CH [6]<< 7);
    sbus_buf[ 8] = (uint8_t)(CH[ 6] >> 1);
    sbus_buf[ 9] = (uint8_t)(CH[ 6] >> 9 | CH [7]<< 2);
    sbus_buf[10] = (uint8_t)(CH[ 7] >> 6 | CH [8]<< 5);
    sbus_buf[11] = (uint8_t)(CH[ 8] >> 3);

    sbus_buf[12] = (uint8_t)(CH[ 9] & 0xFF);
    sbus_buf[13] = (uint8_t)(CH[ 9] >> 8 | CH [10]<<3);
    sbus_buf[14] = (uint8_t)(CH[10] >> 5 | CH [11]<<6);
    sbus_buf[15] = (uint8_t)(CH[11] >> 2);
    sbus_buf[16] = (uint8_t)(CH[11] >> 10| CH [12]<<1);
    sbus_buf[17] = (uint8_t)(CH[12] >> 7 | CH [13]<<4);
    sbus_buf[18] = (uint8_t)(CH[13] >> 4 | CH [14]<<7);
    sbus_buf[19] = (uint8_t)(CH[14] >> 1);
    sbus_buf[20] = (uint8_t)(CH[14] >> 9 | CH [15]<<2);
    sbus_buf[21] = (uint8_t)(CH[15] >> 6 | CH [16]<<5);
    sbus_buf[22] = (uint8_t)(CH[16] >> 3);
    sbus_buf[23] = 0x00;                         // [flags]
    sbus_buf[24] = 0x00;						 // [endbyte]

    HAL_UART_Transmit(&USED_HUART, sbus_buf,25,100);
}


/*
dead zone
@ SPEED_MID
CH[1] +- 33
CH[2] +- 33
CH[4] +- 20
*/
