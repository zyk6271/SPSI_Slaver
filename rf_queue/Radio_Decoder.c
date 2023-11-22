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
#include "led.h"

#define DBG_TAG "RF_DE"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

rf_status rf_pipe_status;
rf_status rf_lora_status;

void rf_lora_decoder(int rssi,int snr,uint8_t *rx_buffer,uint8_t rx_len)
{
    Radio_Frame_Format *recv_buffer = rx_buffer;
    uint8_t target_id_check = rt_memcmp(recv_buffer->target_id, rf_device_id_get(), 4);
    uint8_t device_id_check = rt_memcmp(recv_buffer->device_id, rf_target_id_get(), 4);
    if(target_id_check == 0 && device_id_check == 0)
    {
        remote_control(recv_buffer->value);
        led_transmitter_blink();
        rf_lora_enqueue(recv_buffer->control,recv_buffer->value);
    }
}

void rf_pipe_decoder(int rssi,int snr,uint8_t *rx_buffer,uint8_t rx_len)
{
    Radio_Frame_Format *recv_buffer = &rx_buffer[1];
    uint8_t target_id_check = rt_memcmp(recv_buffer->target_id, rf_device_id_get(), 4);
    uint8_t device_id_check = rt_memcmp(recv_buffer->device_id, rf_target_id_get(), 4);
    if(target_id_check == 0 && device_id_check == 0)
    {
        remote_control(recv_buffer->value);
        led_transmitter_blink();
        rf_pipe_enqueue(recv_buffer->control, recv_buffer->value);
    }
}

void rf_pipe_rx_callback(int rssi,int snr,uint8_t *rx_buffer,uint8_t rx_len)
{
    LOG_D("RF PIPE is %s,RSSI is %d\r\n",rx_buffer,rssi);
    rf_pipe_decoder(rssi,snr,rx_buffer,rx_len);
}

void rf_lora_rx_callback(int rssi,int snr,uint8_t *rx_buffer,uint8_t rx_len)
{
    LOG_D("RF LORA is %s,RSSI is %d\r\n",rx_buffer,rssi);
    rf_lora_decoder(rssi,snr,rx_buffer,rx_len);
}
