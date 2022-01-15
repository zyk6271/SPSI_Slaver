/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-16     RT-Thread    first version
 */

#include <rtthread.h>
#include <stdint.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    led_Init();
    led_transmitter_off();
    rf_433_start();
    rf_4068_start();
    RadioQueueInit();
    work_init();
    while (1)
    {
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
