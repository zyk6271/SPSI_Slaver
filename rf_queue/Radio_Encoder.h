/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-22     Rick       the first version
 */
#ifndef RADIO_RADIO_ENCODER_H_
#define RADIO_RADIO_ENCODER_H_

#include <stdint.h>

typedef struct
{
    char device_id[4];
    char target_id[4];
    uint8_t control;
    uint8_t value;
}Radio_Frame_Format;

typedef struct
{
    uint8_t rf_id;
    uint8_t retry;
    uint8_t alive;
    uint8_t received;
    uint8_t test_received;
    int rssi;
    int snr;
    uint8_t rssi_level;
    uint8_t startup;
}rf_status;

void RadioEnqueue(uint32_t Taget_Id,uint8_t counter,uint8_t Command,uint8_t Data);
void RadioQueue_Init(void);

#endif /* RADIO_RADIO_ENCODER_H_ */
