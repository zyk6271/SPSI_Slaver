/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-21     Rick       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include "Radio_Encoder.h"
#include "pin_config.h"

#define DBG_TAG "work"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

rt_timer_t valve_timer = RT_NULL;

static uint8_t psi_status = 2;
static uint8_t valve_status = 2;
static uint8_t psi_lost = 0;

void valve_control(uint8_t value)
{
    if(valve_status != value)
    {
        valve_status = value;
        switch(value)
        {
        case 0://关
            rt_pin_write(RELAY,0);
            led_transmitter_off();
            led_receiver_warning();
            break;
        case 1://开
            rt_pin_write(RELAY,1);
            led_transmitter_on();
            led_receiver_on();
            break;
        case 2://超时关
            rt_pin_write(RELAY,0);
            led_transmitter_lost();
            led_receiver_lost();
            break;
        }
        LOG_I("valve is write to %d\r\n",value);
    }
}

void remote_control(uint8_t value)
{
    beep_stop();
    if(psi_lost)
    {
        psi_lost = 0;
        valve_control(value);
        return;
    }
    if(psi_status != value)
    {
        psi_status = value;
        valve_control(value);
    }
}

void valve_timeout(void *parameter)
{
    psi_lost = 1;
    beep_start(2);
    remote_control(0);
}

void work_init(void)
{
    rt_pin_mode(RELAY, PIN_MODE_OUTPUT);
    rt_pin_write(RELAY,0);

    valve_timer = rt_timer_create("valve_timeout", valve_timeout, RT_NULL, 60*1000, RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
    led_receiver_off();
}
