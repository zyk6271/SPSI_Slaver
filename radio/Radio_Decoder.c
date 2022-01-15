/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-22     Rick       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include "drv_spi.h"
#include <string.h>
#include "AX5043.h"
#include "Radio_Decoder.h"
#include "Radio_Encoder.h"
#include "pin_config.h"
#include "Radio_Drv.h"

#define DBG_TAG "RF_DE"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern uint32_t Self_ID;
extern uint32_t Target_ID;

void Solve_433(uint8_t *rx_buffer,uint8_t rx_len)
{
    Message Rx_message;
    if(rx_buffer[rx_len-1]=='M')
     {
         sscanf((const char *)&rx_buffer[1],"M{%ld,%ld,%d,%d}M",&Rx_message.Target_ID,&Rx_message.From_ID,&Rx_message.Command,&Rx_message.Data);
         if(Rx_message.Target_ID==Self_ID && Rx_message.From_ID==Target_ID)
         {
             switch(Rx_message.Command)
             {
             case 0://心跳
                 rf_433_Enqueue(Rx_message.From_ID,0,Rx_message.Data);
                 break;
             case 1://button
                 rf_433_Enqueue(Rx_message.From_ID,1,Rx_message.Data);
                 led_transmitter_blink(Rx_message.Data);
                 break;
             case 2://Control
                 rf_433_Enqueue(Rx_message.From_ID,2,Rx_message.Data);
                 break;
             case 3://
                 rf_433_Enqueue(Rx_message.From_ID,2,Rx_message.Data);
                 break;
             }
             remote_control(Rx_message.Data);
         }
     }
}
void Solve_4068(uint8_t *rx_buffer,uint8_t rx_len)
{
    Message Rx_message;
    if(rx_buffer[rx_len-1]=='M')
     {
         sscanf((const char *)&rx_buffer[1],"M{%ld,%ld,%d,%d}M",&Rx_message.Target_ID,&Rx_message.From_ID,&Rx_message.Command,&Rx_message.Data);
         if(Rx_message.Target_ID==Self_ID && Rx_message.From_ID==Target_ID)
         {
             switch(Rx_message.Command)
             {
             case 0://心跳
                 rf_4068_Enqueue(Rx_message.From_ID,0,Rx_message.Data);
                 break;
             case 1://button
                 rf_4068_Enqueue(Rx_message.From_ID,1,Rx_message.Data);
                 led_transmitter_blink(Rx_message.Data);
                 break;
             case 2://Control
                 rf_4068_Enqueue(Rx_message.From_ID,2,Rx_message.Data);
                 break;
             case 3:
                 rf_4068_Enqueue(Rx_message.From_ID,2,Rx_message.Data);
                 break;
             }
             remote_control(Rx_message.Data);
         }
     }
}
void rf433_rx_callback(int rssi,uint8_t *rx_buffer,uint8_t rx_len)
{
    LOG_D("RX 433 is %s,RSSI is %d\r\n",rx_buffer,rssi);
    switch(rx_buffer[1])
    {
    case 'M':
        Solve_433(rx_buffer,rx_len-1);
        break;
    }
}
void rf4068_rx_callback(int rssi,uint8_t *rx_buffer,uint8_t rx_len)
{
    LOG_D("RX 4068 is %s,RSSI is %d\r\n",rx_buffer,rssi);
    switch(rx_buffer[1])
    {
    case 'M':
        Solve_4068(rx_buffer,rx_len-1);
        break;
    }
}
