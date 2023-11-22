/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-23     RT-Thread    first version
 */

#include <rtthread.h>
#include <stm32wlxx.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    led_Init();
    rf_enqueue_init();
    rf_lora_init();
    rf_pipe_init();
    work_init();
    while (1)
    {
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
