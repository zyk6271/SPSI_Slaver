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
#include "Flashwork.h"
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
         sscanf((const char *)&rx_buffer[1],"M{%ld,%ld,%d,%d,%d}M",&Rx_message.Target_ID,&Rx_message.From_ID,&Rx_message.Counter,&Rx_message.Command,&Rx_message.Data);
         if(Rx_message.Target_ID==Self_ID && Rx_message.From_ID==Target_ID)
         {
             LOG_I("NormalSolve verify ok\r\n");
             switch(Rx_message.Command)
             {
             case 0://心跳
                 rf_433_Enqueue(Rx_message.From_ID,0,1);
                 break;
             case 1://PSI
                 //打开关闭PSI
                 break;
             case 2://暂停
                 //暂停30s定时器
                 break;
             }
         }
     }
}
void Solve_4068(uint8_t *rx_buffer,uint8_t rx_len)
{
    Message Rx_message;
    if(rx_buffer[rx_len-1]=='M')
     {
         sscanf((const char *)&rx_buffer[1],"M{%ld,%ld,%d,%d,%d}M",&Rx_message.Target_ID,&Rx_message.From_ID,&Rx_message.Counter,&Rx_message.Command,&Rx_message.Data);
         if(Rx_message.Target_ID==Self_ID && Rx_message.From_ID==Target_ID)
         {
             LOG_I("NormalSolve verify ok\r\n");
             switch(Rx_message.Command)
             {
             case 0://心跳
                 rf_4068_Enqueue(Rx_message.From_ID,0,1);
                 break;
             case 1://PSI
                 //打开关闭PSI
                 break;
             case 2://暂停
                 //暂停30s定时器
                 break;
             }
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
