#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "MDZ_Board.h"


/** @brief Interrupt handler function for the Timer 0 peripheral.
 */
 
void TIMER2_IRQHandler(void)
{
	
	//if( KEY1_Down()== 0){
		//TODO 
		//LED_ON(LED_GREEN);
		LED_ON(LED_RED);
	//}	
	
	NRF_TIMER0->EVENTS_COMPARE[0] = 0;
		
}



static void timer2_init(void)
{
	/*
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTART    = 1;
		while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0){}
	*/
	
			
			

	
		
		    // Set the timer in Timer Mode
    NRF_TIMER2->MODE      = TIMER_MODE_MODE_Timer; 
    NRF_TIMER2->PRESCALER = 9;
    
	
    // 24-bit mode
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;  

		NRF_TIMER2->TASKS_CLEAR=1;
		
    // Enable interrupt for COMPARE[0]
    NRF_TIMER2->INTENSET    = (1UL << TIMER_INTENSET_COMPARE0_Pos);
    NRF_TIMER2->CC[0]       = 1000UL*31;
		NRF_TIMER2->CC[0]      += 1000UL/4;
   // NRF_TIMER2->TASKS_START = 1; // Start clocks
		
		
		

		
    
}


void TIMER2_Init(){
	// DEBOUNCE_INPUT_SAMPLING_FREQ is in hertz, Multiply by 1000 to get milliseconds
    //timer1_cc0_period = MS_TO_TIMER1_TICKS((1000 * 1 / DEBOUNCE_INPUT_SAMPLING_FREQ)*1000);
	
		//__disable_irq();
	
    timer2_init();

    // Enable Interrupt for the timer in the core
    NVIC_EnableIRQ(TIMER2_IRQn); 
    __enable_irq();
	

}




