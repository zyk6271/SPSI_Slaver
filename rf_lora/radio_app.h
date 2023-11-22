/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-15     Rick       the first version
 */
#ifndef RADIO_RADIO_APP_H_
#define RADIO_RADIO_APP_H_

/*
 * Config
 */
#define TX_RX_FREQUENCE_OFFSET                      0            // 0           TX = RX

#define LORA_TX_OUTPUT_POWER                        14          //14dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       10         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE          false
#define LORA_IQ_INVERSION_ON_DISABLE                false

/*
 * Radio Config
 */
#define RF_FREQUENCY                                470900000
#define TX_OUTPUT_POWER                             10
#define RX_TIMEOUT_VALUE                            0
#define TX_TIMEOUT_VALUE                            3000

void RF_Init(void);

#endif /* RADIO_RADIO_APP_H_ */
