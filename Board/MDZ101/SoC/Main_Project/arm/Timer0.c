#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "MDZ_Board.h"

//#define DEBOUNCE_TIME_IN_MS             (50U)                                   /**< Debounce timer in milliseconds */
#define DEBOUNCE_INPUT_SAMPLING_FREQ    (60U)                                   /**< Input sampling frequency in Hertz */
#define TIMER0_PRESCALER                (9UL)                                   /**< Timer 0 prescaler */
#define TIMER0_CLOCK                    (SystemCoreClock >> TIMER0_PRESCALER)   /**< Timer clock frequency */
#define MS_TO_TIMER0_TICKS(ms)          ((1000000UL * ms) / (TIMER0_CLOCK))     /**< Converts milliseconds to timer ticks */
//#define MAX_BUTTONS                     (8U)                                    /**< Maximum number of buttons in use */

static uint_fast16_t timer0_cc0_period;     /**< Period between debouncer input reads. */
//static deb_t button[MAX_BUTTONS];           /**< Debounced button state holder */




/** @brief Interrupt handler function for the Timer 0 peripheral.
 */
void TIMER0_IRQHandler(void)
{
	

  if( KEY1_Down()== 0){
		//TODO 
		LED_ON(LED_GREEN);
	}	
	
		
}


/** @brief Function for initializing Timer 0.
 */
static void timer0_init(void)
{
    // Set the timer in Timer Mode
    //NRF_TIMER1->MODE      |= TIMER_MODE_MODE_Timer;
		NRF_TIMER0->MODE      |= TIMER_MODE_MODE_Pos;	
    NRF_TIMER0->PRESCALER = TIMER0_PRESCALER;
    
    // 24-bit mode
    NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_24Bit;  

    // Enable interrupt for COMPARE[0]
    NRF_TIMER0->INTENSET    = (1UL << TIMER_INTENSET_COMPARE0_Pos);
    NRF_TIMER0->CC[0]       = timer0_cc0_period;
    NRF_TIMER0->TASKS_START = 1; // Start clocks
}


void TIMER0_Init(){
	// DEBOUNCE_INPUT_SAMPLING_FREQ is in hertz, Multiply by 1000 to get milliseconds
    timer0_cc0_period = MS_TO_TIMER0_TICKS((1000 * 1 / DEBOUNCE_INPUT_SAMPLING_FREQ)*1000);
    timer0_init();

    // Enable Interrupt for the timer in the core
    NVIC_EnableIRQ(TIMER0_IRQn); 
    __enable_irq();
}


