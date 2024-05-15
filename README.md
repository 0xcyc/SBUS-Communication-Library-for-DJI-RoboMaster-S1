# SBUS Communication Library for DJI RoboMaster S1
## 主要功能
这个库提供了一个通过SBUS协议控制RoboMaster S1运动的接口。

本次测试基于STM32f4系列的单片机，使用HAL库进行开发。

特别地，测试过程中只使用了发送函数 `Sbus_update(void)`，只有发送函数经过验证，可以较好的控制RMS1运动；接收函数 `Sbus_Data_Decoding(void)`是按照发送函数的逻辑编写的，但**并没有验证**。


## 关于S.BUS
SBUS是FUTABA提出的舵机控制总线，全称Serial Bus
- 采用通信波特率为 `100kBit/s`
- 电平逻辑反转：高电平为逻辑低（0）、低电平为逻辑高（1）
- 通信速率：每`14ms`(模拟模式)或`7ms`(高速模式)发送一帧数据
- 一帧包括：1位起始位 + 8位数据位 + 偶校验位 + 2位停止位


## 宏定义
- `SPEED_FAST`、`SPEED_MID`、`SPEED_SLOW`：定义了不同的速度级别。
- `CHASSIS_FREE`、`CHASSIS_SLAVE`：定义了底盘的两种模式。
- `USED_HUART`：定义了使用的UART端口。

## 变量
- `sbus_buf`：用于存储SBUS数据的缓冲区。
- `CH`：用于存储通道数据的数组。

## 函数
- `Sbus_Init(void)`：初始化SBUS通信，设置通道的默认值。
- `Sbus_Data_Decoding(void)`：解码接收到的SBUS数据，将数据存储到`CH`数组中。
- `Sbus_update(void)`：更新`CH`数组中的数据，并将数据编码为SBUS格式发送出去。

## 注意事项

- 在使用此库之前，需要确保已经正确配置了UART硬件。
- 在使用此库时，需要根据实际情况修改`USED_HUART`宏定义，以指向正确的UART端口。

[RoboMaster S1 用户手册](https://dl.djicdn.com/downloads/robomaster-s1/20220429UM/RoboMaster_S1_User_Manual_v1.8_CHS.pdf)

___
`English Version`

## Main Features
This library provides an interface for controlling the movement of RoboMaster S1 through the SBUS protocol.

This test is based on the STM32f4 series microcontroller and developed using the HAL library.

Specifically, only the send function `Sbus_update(void)` was used during the test. Only the send function has been verified and can control the movement of RMS1 well; the receive function `Sbus_Data_Decoding(void)` is written according to the logic of the send function, but **has not been verified**.

## About S.BUS
SBUS is a servo control bus proposed by FUTABA, officially known as Serial Bus
- It uses a communication baud rate of `100 000Bit/s`
- Logic level inversion: high level is logic low (0), low level is logic high (1)
- Communication rate: send a frame of data every `14ms` (analog mode) or `7ms` (high-speed mode)
- A frame includes: 1 start bit + 8 data bits + even parity bit + 2 stop bits

## Macro Definitions
- `SPEED_FAST`, `SPEED_MID`, `SPEED_SLOW`: define different speed levels.
- `CHASSIS_FREE`, `CHASSIS_SLAVE`: define two modes of the chassis.
- `USED_HUART`: defines the UART port in use.

## Variables
- `sbus_buf`: a buffer for storing SBUS data.
- `CH`: an array for storing channel data.

## Functions
- `Sbus_Init(void)`: initializes SBUS communication and sets the default values for the channels.
- `Sbus_Data_Decoding(void)`: decodes the received SBUS data and stores the data in the `CH` array.
- `Sbus_update(void)`: updates the data in the `CH` array and encodes the data into SBUS format for transmission.

## Precautions
- Before using this library, make sure that the UART hardware has been correctly configured.
- When using this library, you need to modify the `USED_HUART` macro definition according to the actual situation to point to the correct UART port.

[RoboMaster S1 User Manual](https://dl.djicdn.com/downloads/robomaster-s1/20220429UM/RoboMaster_S1_User_Manual_v1.8_EN.pdf)
