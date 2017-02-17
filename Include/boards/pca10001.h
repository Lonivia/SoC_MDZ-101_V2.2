/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef PCA10001_H
#define PCA10001_H

#include "nrf_gpio.h"


/*
#define LED_START      18
#define LED_0          18
#define LED_1          19
#define LED_2          20
#define LED_3          21
#define LED_4          22
#define LED_STOP       22

#define BUTTON_START   16
#define BUTTON_0       16
#define BUTTON_1       17
#define BUTTON_STOP    17
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP


#define LED_RED        23
#define LED_GREEN      25
*/




#define LED_0       23
#define LED_1       25

#define LED_START      23
#define LED_RED        23
#define LED_GREEN      25
#define LED_STOP       25

#define BUTTON_START   15
#define BUTTON_0       15
#define BUTTON_1       16
#define BUTTON_STOP    16
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP



//Board
#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
//Module
//#define RX_PIN_NUMBER  5
//#define TX_PIN_NUMBER  6
//#define CTS_PIN_NUMBER 7
//#define RTS_PIN_NUMBER 12
//#define HWFC           true
#define HWFC           false
	


/*

void	LED_Init(uint32_t LED_START1,uint32_t LED_STOP1);
//void  TIMER0_Init(void);
void  TIMER1_Init(void);
void  TIMER2_Init(void);

void LED_ON(uint32_t pin_num);
void LED_OFF(uint32_t pin_num);
void KEY_Init(void);
void ADC_Init(void);
void UART_Init(void);

uint8_t KEY1_Down(void);
void radio_configure(void);
void Radio_init(void);
void Radio_Tx(void);


extern int Timer_i;
extern int Timer_i2;
extern int Key_Long;
extern uint8_t packet[4];

extern int Long_press;

extern uint8_t ID[4];
extern	uint8_t i;
extern	uint32_t * p_spi_base_address;
*/

extern int Timer_i;

#endif
