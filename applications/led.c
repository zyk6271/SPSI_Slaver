/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-23     Tobby       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include "Pin_Config.h"
#include "signal_led.h"
#include "led.h"

#define DBG_TAG "LED"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static uint8_t beep_mode;

rt_thread_t led_thread = RT_NULL;

static led_t *LED1_G = RT_NULL;
static led_t *LED1_R = RT_NULL;
static led_t *LED1_B = RT_NULL;
static led_t *LED2_G = RT_NULL;
static led_t *LED2_R = RT_NULL;
static led_t *LED2_B = RT_NULL;
static led_t *BEEP = RT_NULL;
static led_t *WARN_BEEP = RT_NULL;

//定义内存操作函数接口
led_mem_opreation_t led_mem_opreation;

/*************LED1***************/
static void LED1_G_ON(void *param)
{
    ws2812b_green(0, 1);
}

static void LED1_G_OFF(void *param)
{
    ws2812b_green(0, 0);
}

static void LED1_R_ON(void *param)
{
    ws2812b_red(0, 1);
}

static void LED1_R_OFF(void *param)
{
    ws2812b_red(0, 0);
}

static void LED1_B_ON(void *param)
{
    ws2812b_blue(0, 1);
}

static void LED1_B_OFF(void *param)
{
    ws2812b_blue(0, 0);
}

/*************LED2***************/
static void LED2_G_ON(void *param)
{
    ws2812b_green(1, 1);
}

static void LED2_G_OFF(void *param)
{
    ws2812b_green(1, 0);
}

static void LED2_R_ON(void *param)
{
    ws2812b_red(1, 1);
}

static void LED2_R_OFF(void *param)
{
    ws2812b_red(1, 0);
}

static void LED2_B_ON(void *param)
{
    ws2812b_blue(1, 1);
}

static void LED2_B_OFF(void *param)
{
    ws2812b_blue(1, 0);
}

static void BEEP_ON(void *param)
{
    rt_pin_mode(BUZZER, PIN_MODE_OUTPUT);
    rt_pin_write(BUZZER, PIN_HIGH);
}

static void BEEP_OFF(void *param)
{
    rt_pin_mode(BUZZER, PIN_MODE_OUTPUT);
    rt_pin_write(BUZZER, PIN_LOW);
}

static void led_run(void *parameter)
{
    ws2812b_init();
    while (1)
    {
        rt_thread_mdelay(LED_TICK_TIME);
        led_ticks();
        RGB_SendArray();
    }
}

void led_Init(void)
{
    led_mem_opreation.malloc_fn = (void* (*)(size_t)) rt_malloc;
    led_mem_opreation.free_fn = rt_free;
    led_set_mem_operation(&led_mem_opreation);

    LED1_G = led_create(LED1_G_ON, LED1_G_OFF, NULL);
    LED1_R = led_create(LED1_R_ON, LED1_R_OFF, NULL);
    LED1_B = led_create(LED1_B_ON, LED1_B_OFF, NULL);

    LED2_G = led_create(LED2_G_ON, LED2_G_OFF, NULL);
    LED2_R = led_create(LED2_R_ON, LED2_R_OFF, NULL);
    LED2_B = led_create(LED2_B_ON, LED2_B_OFF, NULL);

    BEEP = led_create(BEEP_ON, BEEP_OFF, NULL);
    WARN_BEEP = led_create(BEEP_ON, BEEP_OFF, NULL);

    led_thread = rt_thread_create("led_thread", led_run, RT_NULL, 512, 15, 10);
    rt_thread_startup(led_thread);
}

void led_transmitter_lost(void)
{
    led_stop(LED1_G);
    led_set_mode(LED1_R, LOOP_PERMANENT, "1,0,");
    led_start(LED1_R);
}

void led_transmitter_off(void)
{
    led_set_mode(LED1_R,LOOP_PERMANENT,"1,0,");
    led_set_mode(LED1_G,LOOP_PERMANENT,"1,0,");
    led_start(LED1_G);
    led_start(LED1_R);
}

void led_transmitter_on(void)
{
    led_stop(LED1_R);
    led_set_mode(LED1_G, LOOP_PERMANENT, "1,0,");
    led_start(LED1_G);
}

void led_transmitter_blink(void)
{
    led_set_mode(LED1_B, 1, "100,100,");
    led_start(LED1_B);
}

void led_receiver_lost(void)
{
    led_stop(LED2_G);
    led_set_mode(LED2_R, LOOP_PERMANENT, "1,0,");
    led_start(LED2_R);
    led_stop(WARN_BEEP);
}

void led_receiver_off(void)
{
    led_set_mode(LED2_R,LOOP_PERMANENT,"1,0,");
    led_set_mode(LED2_G,LOOP_PERMANENT,"1,0,");
    led_start(LED2_G);
    led_start(LED2_R);
}

void led_receiver_on(void)
{
    led_stop(LED2_R);
    led_set_mode(LED2_G, LOOP_PERMANENT, "1,0,");
    led_start(LED2_G);
    led_stop(WARN_BEEP);
}

void led_receiver_warning(void)
{
    led_set_mode(LED2_R, LOOP_PERMANENT,
            "50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50\
                                    ,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,2000,");
    led_set_mode(WARN_BEEP, LOOP_PERMANENT,
            "50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50\
                                    ,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,2000,");
    led_stop(LED2_G);
    led_start(LED2_R);
    led_start(WARN_BEEP);
}


void beep_start(uint8_t select)
{
    if (select != beep_mode)
    {
        beep_mode = select;
        led_stop(BEEP);
        switch(select)
        {
        case 0://10s/1声
            led_set_mode(BEEP,LOOP_PERMANENT,"200,10000,");
            break;
        case 1://5s/2声
            led_set_mode(BEEP,LOOP_PERMANENT,"200,200,200,5000,");
            break;
        case 2://5s/3声
            led_set_mode(BEEP,LOOP_PERMANENT,"200,200,200,200,200,5000,");
            break;
        }
        led_start(BEEP);
    }
}

void beep_stop(void)
{
    beep_mode = 0;
    led_stop(BEEP);
}
