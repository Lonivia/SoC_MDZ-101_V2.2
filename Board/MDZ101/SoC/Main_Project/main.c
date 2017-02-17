#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf51_bitfields.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"
#include "simple_uart.h"
//#include "boards.h"
#include "ble_error_log.h"
#include "ble_debug_assert_handler.h"
#include "MDZ_Board.h"
#include "Flash.h"
#include "spi_master.h"
#include "ADS1248.h"
#include "nrf_delay.h"
#include "key.h"





uint32_t *Flash_SPI;
uint32_t *ADS1248_SPI;

int main(void)
{
	
	
	
	nrf_gpio_cfg_output(valve);
			nrf_gpio_cfg_output(pump);
	nrf_gpio_pin_clear(pump);
			nrf_gpio_pin_clear(valve);
	
	for(int initi=14;initi<=23;initi++){
		nrf_gpio_cfg_output(initi);
		nrf_gpio_pin_clear(initi);
	}

			
		nrf_delay_us(1000000);

	
    // Initialize
    leds_init();
	
	LED_OFF(LED_GREEN);
		LED_OFF(LED_RED);
	nrf_delay_us(100000);
	
    timers_init();
    buttons_init();
    uart_init();
    ble_stack_init();
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    sec_params_init();
		KEY_Init();
		ADC_Init();
	  
	

	
		Flash_SPI=spi_master_init(SPI1, SPI_MODE3, 0);                    
		ADS1248_SPI=spi_master_init(SPI0, SPI_MODE1, 0); 
		
		InitADS1248(ADS1248_SPI);
		
		Flash_init(Flash_SPI);
		
		AD_Wakeup(ADS1248_SPI);
		Flash_Wakeup(Flash_SPI);
		
		
		LED_ON(LED_GREEN);
		LED_ON(LED_RED);
		nrf_delay_us(300000);
	
		
		
		//Flash_init(Flash_SPI);
		
		LED_OFF(LED_GREEN);		
		//nrf_delay_us(1000000);
		
			
			
 timer1_init();
		TIMER1_Enable();
		

		

 
    for (;;)
    {
			//LED_ON(LED_GREEN);
			MainWork();
    }
}

/** 
 * @}
 */
