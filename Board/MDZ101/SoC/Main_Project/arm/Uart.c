#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "simple_uart.h"
#include "MDZ_Board.h"



void UART_Init(){
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
  simple_uart_putstring((const uint8_t *)" \n\rStart: ");
}

