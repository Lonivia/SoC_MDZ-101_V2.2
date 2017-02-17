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
#include "nrf_delay.h"
#include "MDZ_Board.h"
#include "Flash.h"
#include "ADS1248.h"



static ble_gap_sec_params_t             m_sec_params;                               
static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID; 

//static ble_nus_t                        m_nus;    

int timeouttick=0;

/**@brief     Error handler function, which is called when an error has occurred.
 *
 * @warning   This handler is an example only and does not fit a final product. You need to analyze
 *            how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
	//delete reset here!!!!!!!!!!!!!!!!!!!!!!
}


/**@brief       Assert macro callback function.
 *
 * @details     This function will be called in case of an assert in the SoftDevice.
 *
 * @warning     This handler is an example only and does not fit a final product. You need to
 *              analyze how your product is supposed to react in case of Assert.
 * @warning     On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief   Function for the LEDs initialization.
 *
 * @details Initializes all LEDs used by this application.
 */
void leds_init(void)
{
    nrf_gpio_cfg_output(ADVERTISING_LED_PIN_NO);
    nrf_gpio_cfg_output(CONNECTED_LED_PIN_NO);
}


/**@brief   Function for Timer initialization.
 *
 * @details Initializes the timer module.
 */
void timers_init(void)
{
    // Initialize timer module
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
}


/**@brief   Function for the GAP initialization.
 *
 * @details This function will setup all the necessary GAP (Generic Access Profile)
 *          parameters of the device. It also sets the permissions and appearance.
 */
void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief   Function for the Advertising functionality initialization.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;
    
    ble_uuid_t adv_uuids[] = {{BLE_UUID_NUS_SERVICE, m_nus.uuid_type}};

    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = false;
    advdata.flags.size              = sizeof(flags);
    advdata.flags.p_data            = &flags;

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = adv_uuids;
    
    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);
}


/**@brief    Function for handling the data from the Nordic UART Service.
 *
 * @details  This function will process the data received from the Nordic UART BLE Service and send
 *           it to the UART module.
 */
/**@snippet [Handling the data received over BLE] */

//int testTime=0;

void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
	/*
    for (int i = 0; i < length; i++)
    {
        simple_uart_put(p_data[i]);
    }
    simple_uart_put('\n');
	*/
		
		//==============================================================================
		//======================add TXble here==========================================
		//==============================================================================
	/*
		if(p_data[3]=='0'){
					ble_nus_send_string(&m_nus, "DATA_Not_Ready!", 15);
			}
		if(p_data[3]=='1'){
					LED_ON(LED_RED);
			}
		if(p_data[3]=='2'){
			char buff[50];
			sprintf((char *)buff,"ADC-%d---",nrf_adc_read());
			ble_nus_send_string(&m_nus, buff, 15);
		}
		*/
		
		
		BLE_data_handler( p_nus,   p_data,  length);
		
		/*
		testTime++;
		if(testTime>=100000){
			ble_nus_send_string(&m_nus, "test!", 5);
			testTime=0;
		}
		*/
		
}
/**@snippet [Handling the data received over BLE] */


/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void)
{
    uint32_t         err_code;
    ble_nus_init_t   nus_init;
    
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;
    
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing security parameters.
 */
void sec_params_init(void)
{
    m_sec_params.timeout      = SEC_PARAM_TIMEOUT;
    m_sec_params.bond         = SEC_PARAM_BOND;
    m_sec_params.mitm         = SEC_PARAM_MITM;
    m_sec_params.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    m_sec_params.oob          = SEC_PARAM_OOB;  
    m_sec_params.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    m_sec_params.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
}


/**@brief       Function for handling an event from the Connection Parameters Module.
 *
 * @details     This function will be called for all events in the Connection Parameters Module
 *              which are passed to the application.
 *
 * @note        All this function does is to disconnect. This could have been done by simply setting
 *              the disconnect_on_fail config parameter, but instead we use the event handler
 *              mechanism to demonstrate its use.
 *
 * @param[in]   p_evt   Event received from the Connection Parameters Module.
 */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;
    
    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief       Function for handling errors from the Connection Parameters module.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    
    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;
    
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting advertising.
 */
void advertising_start(void)
{
		//LED_OFF(LED_GREEN);
    uint32_t             err_code;
		err_code+=0;
    ble_gap_adv_params_t adv_params;
    
    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));
    
    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = APP_ADV_INTERVAL;
    adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
	
		//memset(&adv_params, 0, sizeof(adv_params));

    err_code = sd_ble_gap_adv_start(&adv_params);
    //APP_ERROR_CHECK(err_code);

    nrf_gpio_pin_set(ADVERTISING_LED_PIN_NO);
}


/**@brief       Function for the Application's S110 SoftDevice event handler.
 *
 * @param[in]   p_ble_evt   S110 SoftDevice event.
 */
void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;
    static ble_gap_evt_auth_status_t m_auth_status;
    ble_gap_enc_info_t *             p_enc_info;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            nrf_gpio_pin_set(CONNECTED_LED_PIN_NO);
            nrf_gpio_pin_clear(ADVERTISING_LED_PIN_NO);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            nrf_gpio_pin_clear(CONNECTED_LED_PIN_NO);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;

            advertising_start();

            break;
            
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, 
                                                   BLE_GAP_SEC_STATUS_SUCCESS, 
                                                   &m_sec_params);
            APP_ERROR_CHECK(err_code);
            break;
            
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
            break;
            
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            p_enc_info = &m_auth_status.periph_keys.enc_info;
            if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div)
            {
                err_code = sd_ble_gap_sec_info_reply(m_conn_handle, p_enc_info, NULL);
                APP_ERROR_CHECK(err_code);
            }
            else
            {
                // No keys found for this device
                err_code = sd_ble_gap_sec_info_reply(m_conn_handle, NULL, NULL);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT)
            { 
							advertising_start();
							timeouttick++;
							
							
							//LED_ON(LED_GREEN);
							if(timeouttick==2){
                nrf_gpio_pin_clear(ADVERTISING_LED_PIN_NO);

                // Configure buttons with sense level low as wakeup source.
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
			//uint32_t err_code = sd_power_system_off(); 
			//APP_ERROR_CHECK(err_code);			

							
                // Go to system-off mode (this function will not return; wakeup will cause a reset)
                err_code = sd_power_system_off(); 
							//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								
                APP_ERROR_CHECK(err_code);
							}
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief       Function for dispatching a S110 SoftDevice event to all modules with a S110
 *              SoftDevice event handler.
 *
 * @details     This function is called from the S110 SoftDevice event interrupt handler after a
 *              S110 SoftDevice event has been received.
 *
 * @param[in]   p_ble_evt   S110 SoftDevice event.
 */
void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_nus_on_ble_evt(&m_nus, p_ble_evt);
    on_ble_evt(p_ble_evt);
}


/**@brief   Function for the S110 SoftDevice initialization.
 *
 * @details This function initializes the S110 SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    // Initialize SoftDevice.
    //SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_250MS_CALIBRATION, false);
		//NRF_CLOCK_LFCLKSRC_XTAL_500_PPM
	SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM, false);
	//!!!!!!!!!
    
    // Subscribe for BLE events.
    uint32_t err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

/**@brief  Function for configuring the buttons.
 */
void buttons_init(void)
{
    nrf_gpio_cfg_sense_input(WAKEUP_BUTTON_PIN,
                             BUTTON_PULL, 
                             NRF_GPIO_PIN_SENSE_LOW);    
}


/**@brief  Function for placing the application in low power state while waiting for events.
 */
void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


/**@brief  Function for initializing the UART module.
 */


void uart_init(void)
{
/*
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
    
    NRF_UART0->INTENSET = UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos;
    
    NVIC_SetPriority(UART0_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(UART0_IRQn);
	*/

}




/**@brief   Function for handling UART interrupts.
 *
 * @details This function will receive a single character from the UART and append it to a string.
 *          The string will be be sent over BLE when the last character received was a 'new line'
 *          i.e '\n' (hex 0x0D) or if the string has reached a length of @ref NUS_MAX_DATA_LENGTH.
 */
//unsigned char *g_ucUART0_RxBuff;
//int g_ucUART0_RxWritePtr=0;




void UART0_IRQHandler(void)
{
	
	
//    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
	 // static uint8_t test[]="aabbcc\n";
   // static uint8_t index = 0;
    //uint32_t err_code;

    /**@snippet [Handling the data received over UART] */

    
	
		//data_array[index] = simple_uart_get();
	
	
/*	
	
		if( NRF_UART0->EVENTS_RXDRDY == 1)		// RXD
  {
		NRF_UART0->EVENTS_RXDRDY = 0;
		g_ucUART0_RxBuff[g_ucUART0_RxWritePtr] = NRF_UART0->RXD;
		g_ucUART0_RxWritePtr++;
		if( g_ucUART0_TxWritePtr > 255 ) g_ucUART0_TxWritePtr = 0;
  }
	
	if(g_ucUART0_TxReadPtr != g_ucUART0_TxWritePtr)		//TXD
	{
		  if (NRF_UART0->EVENTS_TXDRDY==1)
			{
				NRF_UART0->EVENTS_TXDRDY = 0;
				NRF_UART0->TXD = (uint8_t)g_ucUART0_RxBuff[g_ucUART0_TxReadPtr];
				g_ucUART0_TxReadPtr++;
				if( g_ucUART0_TxReadPtr > 255 ) g_ucUART0_TxReadPtr = 0;
			}
	}
	
    index++;
		
*/
		
		
/*
    if ((data_array[index - 1] == '\n') || (index >= (BLE_NUS_MAX_DATA_LEN - 1)))
    {
        err_code = ble_nus_send_string(&m_nus, data_array, index + 1);
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
            APP_ERROR_CHECK(err_code);
        }
        
        index = 0;
    }
		*/
		
		
		/*
		if ((test[index - 1] == '\n') || (index >= (BLE_NUS_MAX_DATA_LEN - 1)))
    {
        err_code = ble_nus_send_string(&m_nus, test, index + 1);
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
            APP_ERROR_CHECK(err_code);
        }
        
        index = 0;
    }
		*/
		
		

    /**@snippet [Handling the data received over UART] */
}
