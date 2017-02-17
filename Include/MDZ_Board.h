#ifndef MDZ_101_H
#define MDZ_101_H

#include "nrf_gpio.h"
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
#include "nrf_adc.h"

#define HW_VER_12 		12
#define HW_VER_13 		13

#if( HW_VER == HW_VER_12 )
	#define LED_0       23
	#define LED_1       25

	#define LED_START      23
	#define LED_RED        23
	#define LED_GREEN      25
	#define LED_STOP       25

	#define BUTTON_START   15
	#define BUTTON_0       15
	//#define BUTTON_1       16
	//#define BUTTON_STOP    16
	#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP
	#define KEY_1  15

	#define AD0    				 1
	#define AD1    				 3



	#define MMOSI    			 28
	#define MMISO    			 29
	#define MMCK    			 26
	#define MMSN    			 27
	




	#define AD_SCLK    		 8
	#define AD_MOSI    		 9
	#define AD_MISO    		 10
	#define AD_DRDY    		 11
	#define AD_CS     		 12
	#define AD_START     	 13
	#define AD_RST      	 14


	#define AD_CH45    		 17

	#define CHRG 2
	



	//#define RX_PIN_NUMBER  4  
	//#define TX_PIN_NUMBER  5 
	//#define CTS_PIN_NUMBER 10
	//#define RTS_PIN_NUMBER 8
	#define   ADS_Start1  nrf_gpio_pin_write(AD_START, 1) 
	#define   ADS_Start0  nrf_gpio_pin_write(AD_START, 0) 
		
	#define   ADS_DRDY1   nrf_gpio_pin_write(AD_DRDY, 1) 
	#define   ADS_DRDY0   nrf_gpio_pin_write(AD_DRDY, 0) 

	#define   ADS_Din1    nrf_gpio_pin_write(AD_MISO, 1) 
	#define   ADS_Din0    nrf_gpio_pin_write(AD_MISO, 0)  

	#define   ADS_Dout    nrf_gpio_pin_read(AD_DRDY)           
		
	#define   ADS_SCLK1   nrf_gpio_pin_write(AD_SCLK, 1)
	#define   ADS_SCLK0   nrf_gpio_pin_write(AD_SCLK, 0)

	#define   ADS_RESET1   nrf_gpio_pin_write(AD_RST, 1) 
	#define   ADS_RESET0   nrf_gpio_pin_write(AD_RST, 0) 

	#define Flash_CE_LOW nrf_gpio_pin_write(MMSN, 0);
	#define Flash_CE_HIGH nrf_gpio_pin_write(MMSN, 1);

#elif(HW_VER == HW_VER_13 )
	#define LED_0       26
	#define LED_1       27

	#define LED_START      26
	#define LED_RED        26
	#define LED_GREEN      27
	#define LED_STOP       27

	#define BUTTON_START   0
	#define BUTTON_0       0

	#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP
	#define KEY_1  0

	#define AD0    				 1
	#define AD1    				 2



	#define MMOSI    			 28
	#define MMISO    			 4
	#define MMCK    			 29
	#define MMSN    			 3
	




	#define AD_SCLK    		 11
	#define AD_MOSI    		 10
	#define AD_MISO    		 9
	#define AD_DRDY    		 8
	#define AD_CS     		 7
	#define AD_START     	 6
	#define AD_RST      	 5


	#define AD_CH45    		 17

	#define CHRG 30
	
		#define CHA 24
	#define CHB 25

	#define RX_PIN_NUMBER  14  
	#define TX_PIN_NUMBER  15 
	#define CTS_PIN_NUMBER 15
	#define RTS_PIN_NUMBER 14
	
	
	#define   ADS_Start1  nrf_gpio_pin_write(AD_START, 1) 
	#define   ADS_Start0  nrf_gpio_pin_write(AD_START, 0) 
		
	#define   ADS_DRDY1   nrf_gpio_pin_write(AD_DRDY, 1) 
	#define   ADS_DRDY0   nrf_gpio_pin_write(AD_DRDY, 0) 

	#define   ADS_Din1    nrf_gpio_pin_write(AD_MISO, 1) 
	#define   ADS_Din0    nrf_gpio_pin_write(AD_MISO, 0)  

	#define   ADS_Dout    nrf_gpio_pin_read(AD_DRDY)           
		
	#define   ADS_SCLK1   nrf_gpio_pin_write(AD_SCLK, 1)
	#define   ADS_SCLK0   nrf_gpio_pin_write(AD_SCLK, 0)

	#define   ADS_RESET1   nrf_gpio_pin_write(AD_RST, 1) 
	#define   ADS_RESET0   nrf_gpio_pin_write(AD_RST, 0) 

	#define Flash_CE_LOW nrf_gpio_pin_write(MMSN, 0);
	#define Flash_CE_HIGH nrf_gpio_pin_write(MMSN, 1);
	
	#define valve 12
	#define pump 13

#endif


#define HWFC           false


void	LED_Init(uint32_t LED_START1,uint32_t LED_STOP1);
void timer1_init(void);
//void  TIMER0_Init(void);
void  TIMER1_Enable(void);
//void  TIMER2_Init(void);

void LED_ON(uint32_t pin_num);
void LED_OFF(uint32_t pin_num);
void KEY_Init(void);
void ADC_Init(void);
void UART_Init(void);

//void radio_configure(void);
//void Radio_init(void);
//void Radio_Tx(void);

//void advertising_start(void);

//extern uint8_t packet[4];

extern int Long_press;

//extern uint8_t ID[4];
//extern	uint8_t i;
extern	uint32_t * p_spi_base_address;

void RTC0_Init(void);



//BLE!!!!!!
#define WAKEUP_BUTTON_PIN               BUTTON_0                                    /**< Button used to wake up the application. */

#define ADVERTISING_LED_PIN_NO          LED_0                                       /**< LED to indicate advertising state. */
#define CONNECTED_LED_PIN_NO            LED_1                                       /**< LED to indicate connected state. */

#define DEVICE_NAME                     "MDZW"                               /**< Name of device. Will be included in the advertising data. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            2                                           /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL               16                                          /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               60                                          /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< slave latency. */
#define CONN_SUP_TIMEOUT                400                                         /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(20000, APP_TIMER_PRESCALER) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (20 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)    /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define SEC_PARAM_TIMEOUT               30                                          /**< Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND                  1                                           /**< Perform bonding. */
#define SEC_PARAM_MITM                  0                                           /**< Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                        /**< No I/O capabilities. */
#define SEC_PARAM_OOB                   0                                           /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE          7                                           /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE          16                                          /**< Maximum encryption key size. */

#define START_STRING                    "Start...\n"                                /**< The string that will be sent over the UART when the application starts. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

void leds_init(void);
void timers_init(void);
void buttons_init(void);
void uart_init(void);
void ble_stack_init(void);
void gap_params_init(void);
void services_init(void);
void advertising_init(void);
void conn_params_init(void);
void sec_params_init(void);

void advertising_start(void);

void power_manage(void);

//void nus_data_handler();

void MainWork(void);

extern int g_ul5msTick;

void LED_Manager(void);
void KEY_Manager(void);
void AD_Manager(void);

extern unsigned char g_ucUART0_RxReadPtr;
extern unsigned char g_ucUART0_RxWritePtr;
extern unsigned char g_ucUART0_TxReadPtr;
extern unsigned char g_ucUART0_TxWritePtr;

extern unsigned char g_ucUART0_RxBuff[256];
extern unsigned char g_ucUART0_TxBuff[256];

void BLE_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

   
extern ble_nus_t                        m_nus; 

void BLE_data_init(void);

extern uint32_t *Flash_SPI;
extern uint32_t *ADS1248_SPI;



extern uint8_t timer_50ms_tick;
extern uint8_t timer_1ms_tick;

extern uint8_t Write_Protection;

extern uint8_t bts_test[];

uint8_t adc_read_bat(void);
uint8_t adc_read_usb(void);

extern int ADtimeout;

extern long int Write_Ptr;
extern long int Read_Ptr;
extern uint8_t Write_PATIENT_NO;
extern uint8_t Read_PATIENT_NO;

extern int Send_i;
extern int AD_i;

extern int g_uiWdtCnt;

extern void ClearWdt( void );

extern uint8_t pumpflag;
extern uint8_t valveflag;
extern uint8_t isvalve;

extern uint8_t isRec;
extern uint8_t isRec2;


#endif
