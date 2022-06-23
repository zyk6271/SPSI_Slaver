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

rt_thread_t work_t = RT_NULL;
rt_timer_t valve_timer = RT_NULL;
rt_sem_t valve_sem = RT_NULL;

uint8_t psi_status=2;
uint8_t valve_status=2;
uint8_t psi_lost=0;

extern uint32_t Target_ID;

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
            led_receiver_off();
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
    rt_sem_release(valve_sem);
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
    valve_control(3);
}
void long_heart(void)
{
    uint32_t time = 30*60*1000;
    rt_timer_control(valve_timer, RT_TIMER_CTRL_SET_TIME, &time);
    rt_timer_start(valve_timer);
    LOG_I("start 30 minutes period\r\n");
}
void work_callback(void *parameter)
{
    uint32_t time;
    rt_pin_mode(RELAY, PIN_MODE_OUTPUT);
    rt_pin_write(RELAY,0);
    while(1)
    {
        if(rt_sem_take(valve_sem,RT_WAITING_FOREVER) == RT_EOK)
        {
            time = 1*60*1000;
            rt_timer_control(valve_timer, RT_TIMER_CTRL_SET_TIME, &time);
            rt_timer_start(valve_timer);
        }
    }
}
void work_init(void)
{
    valve_sem = rt_sem_create("valve_refresh", 0, RT_IPC_FLAG_FIFO);
    valve_timer = rt_timer_create("valve_timeout", valve_timeout, RT_NULL, 1*60*1000, RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
    work_t = rt_thread_create("work", work_callback, RT_NULL, 1024, 10, 10);
    if(work_t != RT_NULL)
    {
        rt_thread_startup(work_t);
    }
}
