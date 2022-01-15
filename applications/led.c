/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-22     Rick       the first version
 */
#include <rtthread.h>
#include <agile_led.h>
#include <stdlib.h>
#include "Pin_Config.h"
#include "led.h"

#define DBG_TAG "LED"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static agile_led_t *LED1_G = RT_NULL;
static agile_led_t *LED1_R = RT_NULL;
static agile_led_t *LED2_G = RT_NULL;
static agile_led_t *LED2_R = RT_NULL;
static agile_led_t *BEEP = RT_NULL;
/*
*   该LED组件经修改,模式字符串的首位为关灯延迟时长
*/
void led_Init(void)
{
    LED1_G = agile_led_create(LED1_G_Pin, PIN_LOW, "0,300,500", -1);
    LED1_R = agile_led_create(LED1_R_Pin, PIN_LOW, "0,300,500", -1);
    LED2_G = agile_led_create(LED2_G_Pin, PIN_LOW, "0,100,100", -1);
    LED2_R = agile_led_create(LED2_R_Pin, PIN_LOW, "0,100,100", -1);
    BEEP = agile_led_create(BUZZER, PIN_HIGH, "0,300,500", -1);
}
void led_transmitter_lost(void)
{
    agile_led_set_light_mode(LED1_R,"300,100",1);
    agile_led_off(LED1_G);
    agile_led_start(LED1_R);
}
void led_transmitter_off(void)
{
    agile_led_set_light_mode(LED1_R,"300,100",1);
    agile_led_set_light_mode(LED1_G,"300,100",1);
    agile_led_start(LED1_G);
    agile_led_start(LED1_R);
}
void led_transmitter_on(void)
{
    agile_led_set_light_mode(LED1_G,"300,100",1);
    agile_led_off(LED1_R);
    agile_led_start(LED1_G);
}
void led_receiver_lost(void)
{
    agile_led_set_light_mode(LED2_R,"300,100",1);
    agile_led_off(LED2_G);
    agile_led_start(LED2_R);
}
void led_receiver_off(void)
{
    agile_led_set_light_mode(LED2_R,"300,100",1);
    agile_led_set_light_mode(LED2_G,"300,100",1);
    agile_led_start(LED2_G);
    agile_led_start(LED2_R);
}
void led_receiver_on(void)
{
    agile_led_set_light_mode(LED2_G,"300,100",1);
    agile_led_off(LED2_R);
    agile_led_start(LED2_G);
}
void led_transmitter_blink(uint8_t valve)
{
    if(valve)
    {
        agile_led_set_light_mode(LED2_G,"100,100,100,100",4);
        agile_led_off(LED2_G);
        agile_led_start(LED2_G);
    }
    else
    {
        agile_led_set_light_mode(LED2_R,"100,100,100,100",4);
        agile_led_set_light_mode(LED2_G,"100,100,100,100",4);
        agile_led_off(LED2_R);
        agile_led_off(LED2_G);
        agile_led_start(LED2_R);
        agile_led_start(LED2_G);
    }
}
void beep_start(uint8_t select)
{
    agile_led_stop(BEEP);
    agile_led_off(BEEP);
    switch(select)
    {
    case 0://10s/1声
        agile_led_set_light_mode(BEEP,"300,200,10000",-1);
        break;
    case 1://5s/2声
        agile_led_set_light_mode(BEEP,"300,200,200,200,5000",-1);
        break;
    case 2://5s/3声
        agile_led_set_light_mode(BEEP,"300,200,200,200,200,200,5000",-1);
        break;
    }
    agile_led_start(BEEP);
}
void beep_stop(void)
{
    agile_led_stop(BEEP);
    agile_led_off(BEEP);
}
