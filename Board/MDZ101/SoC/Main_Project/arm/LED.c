/** @file
* @defgroup MDZ-101_app LED.c
* @{
* @ingroup MDZ-101
*
* @author GLN
* @brief MDZ-101 Application LED_Ctrl file.
*
*
*/

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "MDZ_Board.h"

void LED_Init(uint32_t LED_START1,uint32_t LED_STOP1){
	nrf_gpio_range_cfg_output(LED_START1,LED_STOP1);
}

void LED_ON(uint32_t pin_num){
	nrf_gpio_pin_set(pin_num);
}

void LED_OFF(uint32_t pin_num){
	nrf_gpio_pin_clear(pin_num);
}



