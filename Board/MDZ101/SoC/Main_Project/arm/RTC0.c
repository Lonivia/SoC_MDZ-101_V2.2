#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "simple_uart.h"
#include "MDZ_Board.h"
#include "spi_master_config.h"
#include "AT45DBXX.h"


#include <stdio.h>

#define GPIO_TOGGLE_TICK_EVENT    (LED_0)                                 /**< Pin number to toggle when there is a tick event in RTC. */
#define GPIO_TOGGLE_COMPARE_EVENT (LED_1)                                 /**< Pin number to toggle when there is compare event in RTC. */
#define LFCLK_FREQUENCY           (32768UL)                               /**< LFCLK frequency in Hertz, constant. */
#define RTC_FREQUENCY             (8UL)                                   /**< Required RTC working clock RTC_FREQUENCY Hertz. Changable. */
#define COMPARE_COUNTERTIME       (3UL)                                   /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */
#define COUNTER_PRESCALER         ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1)   /* f = LFCLK/(prescaler + 1) */




/** @brief Function starting the internal LFCLK XTAL oscillator.
 */
static void lfclk_config(void)
{
    NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_LFCLKSTART     = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        //Do nothing.
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}


/** @brief Function for configuring the RTC with TICK to 100Hz and COMPARE0 to 10 sec.
 */
static void rtc_config(void)
{
    NVIC_EnableIRQ(RTC0_IRQn);                                      // Enable Interrupt for the RTC in the core.
    NRF_RTC0->PRESCALER     = COUNTER_PRESCALER;                    // Set prescaler to a TICK of RTC_FREQUENCY.
    NRF_RTC0->CC[0]         = COMPARE_COUNTERTIME * RTC_FREQUENCY;  // Compare0 after approx COMPARE_COUNTERTIME seconds.

    // Enable TICK event and TICK interrupt:
    NRF_RTC0->EVTENSET      = RTC_EVTENSET_TICK_Msk;
    NRF_RTC0->INTENSET      = RTC_INTENSET_TICK_Msk;

    // Enable COMPARE0 event and COMPARE0 interrupt:
    NRF_RTC0->EVTENSET      = RTC_EVTENSET_COMPARE0_Msk;
    NRF_RTC0->INTENSET      = RTC_INTENSET_COMPARE0_Msk;
}


/** @brief Function for Configuring PIN8 and PIN9 as outputs.
 */
static void gpio_config(void)
{
    nrf_gpio_cfg_output(GPIO_TOGGLE_TICK_EVENT);
    nrf_gpio_cfg_output(GPIO_TOGGLE_COMPARE_EVENT);

    nrf_gpio_pin_write(GPIO_TOGGLE_TICK_EVENT, 0);
    nrf_gpio_pin_write(GPIO_TOGGLE_COMPARE_EVENT, 0);
}


/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */
/*
uint8_t ID[4];
uint8_t i=0;


int Key_Count=0;
*/
//int Long_press=0;


void RTC0_IRQHandler()
{
	
}




void RTC0_Init(){
		gpio_config();
    lfclk_config();
    rtc_config();
    
    NRF_RTC0->TASKS_START = 1;
}

