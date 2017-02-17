#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_adc.h"
#include "s110\nrf_soc.h"
#include "MDZ_Board.h"

#include "key.h"
#include "FLASH.h"
#include "ADS1248.h"

void KEY_Init(void)
{	
  nrf_gpio_cfg_input(KEY_1,NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_sense_input(KEY_1, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW); //DETECT signal!!!!
	
	nrf_gpio_cfg_input(CHRG,NRF_GPIO_PIN_PULLUP);

}
	
/*void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} */

void KEY1_Manger(void)		// 1ms
{			
	static unsigned int KeyCnt = 0;
	
	
	if( nrf_gpio_pin_read(KEY_1)== 0 ) 
	{	   
		KeyCnt++;
		if(KeyCnt>=2000)
		
		{
			LED_ON(LED_RED);
			LED_ON(LED_GREEN);
			nrf_gpio_cfg_sense_input(WAKEUP_BUTTON_PIN,
															 BUTTON_PULL,
															 NRF_GPIO_PIN_SENSE_LOW);      
			Flash_Sleep(Flash_SPI);
			AD_Sleep(ADS1248_SPI);
			nrf_delay_us(1000000);	
			
			LED_OFF(LED_GREEN);
			LED_OFF(LED_RED);
			
//NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  
			
			nrf_gpio_pin_clear(MMOSI);
			nrf_gpio_pin_clear(MMISO);
			nrf_gpio_pin_clear(MMCK);
			nrf_gpio_pin_clear(MMSN);
			nrf_gpio_pin_clear(AD_SCLK);
			nrf_gpio_pin_clear(AD_MOSI);
			nrf_gpio_pin_clear(AD_MISO);
			nrf_gpio_pin_clear(AD_DRDY); //!!!»»³Éclear¸üµÍ
			nrf_gpio_pin_clear(AD_CS);
			nrf_gpio_pin_clear(AD_START);
			nrf_gpio_pin_clear(AD_RST);	
			
			
			
			nrf_gpio_pin_clear(pump);
			nrf_gpio_pin_clear(valve);
			
			//nrf_delay_us(1000000);
			NVIC_DisableIRQ(TIMER1_IRQn);
			nrf_delay_us(1000000);
			//NRF_CLOCK->TASKS_HFCLKSTOP = 1;
			uint32_t err_code = sd_power_system_off(); 
			APP_ERROR_CHECK(err_code);			
		}
	}
	else
	{
		KeyCnt = 0;
	}
}

