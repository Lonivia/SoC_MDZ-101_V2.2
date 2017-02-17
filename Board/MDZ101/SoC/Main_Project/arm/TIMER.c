#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "MDZ_Board.h"

#include "BLEhandle.h"
#include "key.h"

uint8_t timer_50ms_tick=0;
uint8_t timer_1ms_tick=0;

uint8_t pumpflag=0;
uint8_t valveflag=0;

/** @brief Interrupt handler function for the Timer 0 peripheral.
 */
 
void TIMER1_IRQHandler(void)   //1ms
{
	static unsigned short usWDTMs, us1ms;
	us1ms++;
	
	if(us1ms>100)
	{
		us1ms = 0;
		timer_50ms_tick = 1;	
	}
	
	timer_1ms_tick++;
	
	//if(FindFlag!=0){
	//ADtimeout++;
	//}
	
	usWDTMs++;
	g_uiWdtCnt++;
	if( usWDTMs >= 2000 )
	{
		if( g_uiWdtCnt >= 2000 )
		{
			//NVIC_SystemReset();
		}
		else
		{
			g_uiWdtCnt = 0;
		}
		usWDTMs = 0;
	}		

	if(pumpflag==0){
		nrf_gpio_pin_clear(pump);
	}else if(pumpflag==1){
		nrf_gpio_pin_set(pump);
	}
	
	if(valveflag==0){
		//nrf_gpio_pin_clear(valve);
	}else if(valveflag==1){
		//nrf_gpio_pin_set(valve);
	}
	
/*	if(usWDTMs>=2000){
		usWDTMs=0;
		if(while_i_flag==0){
			while_i_back=while_i;
			while_i_flag=1;
		}else{
			while_i_now=while_i;
			while_i_flag=0;
			if(while_i_now-while_i_back==0 && while_i_now!=0 && while_i_back!=0){
				//LED_OFF(LED_GREEN);
				//LED_OFF(LED_RED);
				NVIC_SystemReset();
			}
			//while_i_back=0;
			//while_i_now=0;
			//while_i=0;
		}
	}*/
	//KEY1_Manger();
	
	NRF_TIMER1->EVENTS_COMPARE[0] = 0;//////////
	//NRF_TIMER1->EVENTS_COMPARE[1] = 0;
	//NRF_TIMER1->EVENTS_COMPARE[2] = 0;
	//NRF_TIMER1->EVENTS_COMPARE[3] = 0;		
}

void timer1_init(void)
{
	/*
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTART    = 1;
		while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0){}
	*/			

	//sd_clock_hfclk_request();
	//NRF_CLOCK->TASKS_HFCLKSTART    = 1;
		
		    // Set the timer in Timer Mode
    NRF_TIMER1->MODE      = TIMER_MODE_MODE_Timer; 
    NRF_TIMER1->PRESCALER = 0;
    
	
    // 24-bit mode
    NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;  

		NRF_TIMER1->TASKS_CLEAR=1;
			
		//NRF_TIMER1->TASKS_CAPTURE[0]=1;
		
		NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
		//NRF_TIMER1->CC[0]       = 1UL*31;
		//NRF_TIMER1->CC[0]      += 1UL/4;	
		NRF_TIMER1->CC[0]=16000;
		
    // Enable interrupt for COMPARE[0]
    NRF_TIMER1->INTENSET    = (1UL << TIMER_INTENSET_COMPARE0_Pos);

    
    NRF_TIMER1->TASKS_START = 1; // Start clocks    
}

void TIMER1_Enable(){
	// DEBOUNCE_INPUT_SAMPLING_FREQ is in hertz, Multiply by 1000 to get milliseconds
    //timer1_cc0_period = MS_TO_TIMER1_TICKS((1000 * 1 / DEBOUNCE_INPUT_SAMPLING_FREQ)*1000);
	
		//__disable_irq();
	
    //timer1_init();

    // Enable Interrupt for the timer in the core
    NVIC_EnableIRQ(TIMER1_IRQn); ////////////
    __enable_irq();
}


