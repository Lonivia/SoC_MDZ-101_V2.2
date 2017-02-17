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
#include "BLEhandle.h"
#include "nrf_delay.h"
#include "Flash.h"
#include "ADS1248.h"
#include "MainWork.h"
#include "key.h"


struct Header wr_header;
int g_uiWdtCnt=0;
int valve_i=0;

int noBat=0;
int noBattimeout=0;

void ClearWdt( void )
{
	g_uiWdtCnt = 0;
}

void MainWork(){		// 1ms
	
	
	static unsigned int uiLast1msTick = 0;
	
	if( timer_1ms_tick == uiLast1msTick )
	{
		return;
	}
	uiLast1msTick = timer_1ms_tick;	
	ClearWdt();
	
	valve_i++;
	if(valve_i>=1000){
		valve_i=0;
	}
	
	KEY1_Manger();	
	
	if(startBLE==0 && (noBat>0 || (adc_read_usb()<=0xAF && adc_read_usb()<=0xAF) )){
				Key_Count=0;
				enableBLE=0;
		if(noBat<=200){
				LED_ON(LED_GREEN);
				LED_ON(LED_RED);
		}else{
				LED_OFF(LED_GREEN);
				LED_OFF(LED_RED);
		}
		
		noBat++;
		noBattimeout++;
		if(noBat>=400){
			noBat=1;
		}
		if(noBattimeout>=2000){
			LED_ON(LED_RED);
			LED_ON(LED_GREEN);
			nrf_gpio_cfg_sense_input(WAKEUP_BUTTON_PIN,
															 BUTTON_PULL,
															 NRF_GPIO_PIN_SENSE_LOW);      
			Flash_Sleep(Flash_SPI);
			AD_Sleep(ADS1248_SPI);
			nrf_delay_us(500000);	
			
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
				
	}else{
				Key_Count=2;
	}
	
	
	if(Key_Count==2 && enableBLE){
		Key_Count=0;
		LED_OFF(LED_GREEN);
		advertising_start();
		power_manage();
		enableBLE=0;
		startBLE=1;
	}
	

	
	
	if(startBLE){
		//nus_data_handler(p_nus, p_data, length);
		//uint8_t bts_battt[]={0x01,0xAA,0x00,0x00,while_i,Dog};
		//while(ble_nus_send_string(&m_nus, bts_battt, 6)){}
		//LED_ON(LED_GREEN);
		
		if(SendFlag && !ADFlag /*&& isRec2==0*/){
			ADFlag=0;  //When transmit , Cannot start ADcollect!
			if(Send_i==0){
				//uint8_t bts_start[]={0x01,0x44,0x00,0x00,BLE_flag,Write_PATIENT_NO,Read_PATIENT_NO};
				//while(ble_nus_send_string(&m_nus, bts_start, 7)){}
				SN2=0;
				SN1=0;
			}	
			SN2=(Send_i>>8)&0xFF;
			SN1=Send_i&0xFF;
			Read_ADdata(Flash_SPI,readdata); 
			uint8_t bts1[]={BLE_flag,	SN2,SN1,
				(readdata[0].channel[0]&0xff00)>>8,readdata[0].channel[0]&0xFF,
				(readdata[0].channel[1]&0xff00)>>8,readdata[0].channel[1]&0xFF,
				(readdata[0].channel[2]&0xff00)>>8,readdata[0].channel[2]&0xFF,
				(readdata[0].channel[3]&0xff00)>>8,readdata[0].channel[3]&0xFF,
				(readdata[0].channel[4]&0xff00)>>8,readdata[0].channel[4]&0xFF,
				
			};
			while(ble_nus_send_string(&m_nus, bts1, 13)){}
			isRec2=1;
			/*
				uint8_t btsAD_Log[]={0x01,0x43,SN2,SN1,(readdata[0].channel[4]&0xff00)>>8,readdata[0].channel[4]&0xFF};
				while(ble_nus_send_string(&m_nus, btsAD_Log, 6)){}
				*/		
			Send_i++;	
			if(Send_i%40==0){
				LED_ON(LED_RED);
			}else{
				LED_OFF(LED_RED);
			}
			if(Send_i>=2400){
				uint8_t bts_stop[]={0x01,0x45,0x00,0x00,BLE_flag,Write_PATIENT_NO,Read_PATIENT_NO};
				while(ble_nus_send_string(&m_nus, bts_stop, 7)){}
				//Flash_init(Flash_SPI);
				SendFlag=0;
				Send_i=0;
				SN1=0;
				SN2=0;
				wr_header.flag=0x01;
				wr_header.ID=0;
				Write_Header(Flash_SPI,wr_header,1);
				LED_OFF(LED_RED);
			}
		}
		
		if(ADFlag && Write_Protection==0){
			SendFlag=0;
			if(timer_1ms_tick>=16){  ////
				if(AD_i==0){
					uint8_t btsAD_Start[]={0x01,0x37,0x00,0x00,BLE_flag,Write_PATIENT_NO,Read_PATIENT_NO};
					while(ble_nus_send_string(&m_nus, btsAD_Start, 7)){}
					
					wr_header.flag=0x0F;
					wr_header.ID=0;
					Write_Header(Flash_SPI,wr_header,0);
					SN2=0;
					SN1=0;
				}
				SN2=(AD_i>>8)&0xFF;
				SN1=AD_i&0xFF;
				
				ADS(ADS1248_SPI,RxData);				
				Write_ADdata(Flash_SPI,RxData);
				
				/*
				uint8_t btsAD_S[]={0x01,0x77,SN2,SN1,
													(RxData[0]>>8)&0xFF,RxData[0]&0xFF,
														(RxData[1]>>8)&0xFF,RxData[1]&0xFF,
															(RxData[2]>>8)&0xFF,RxData[2]&0xFF,
																(RxData[3]>>8)&0xFF,RxData[3]&0xFF,
																	(RxData[4]>>8)&0xFF,RxData[4]&0xFF};
					while(ble_nus_send_string(&m_nus, btsAD_S, 14)){}
				*/
				
				
				AD_i++;
				timer_1ms_tick=0;

				if(AD_i>=40 && AD_i%40==0){
					//uint8_t btsAD_Log[]={0x01,0x88,
						//(Write_Ptr>>8)&0xFF,Write_Ptr&0xFF};
					//while(ble_nus_send_string(&m_nus, btsAD_Log, 4)){}
					if(AD_LED==0){
						LED_OFF(LED_RED);
						AD_LED=1;
					}else{
						LED_ON(LED_RED);
						AD_LED=0;
					}
				}
			}
			if(AD_i>=2400){
				uint8_t btsAD_Finish[]={0x01,0x42,0x00,0x00,BLE_flag,Write_PATIENT_NO,Read_PATIENT_NO};
				ble_nus_send_string(&m_nus, btsAD_Finish, 7);
				//Flash_init(Flash_SPI);
				AD_i=0;
				ADFlag=0;
				SN2=0;
				SN1=0;
				LED_OFF(LED_RED);
			}
		}
		
/*
		if(Write_Protection==1){
			if(AD_LED==0){
					LED_ON(LED_RED);
					AD_LED=1;
			}else{
					LED_OFF(LED_RED);
					AD_LED=0;
			}
		}
*/
		
		if(StopFlag && AD_i!=0){
			StopFlag=0;
			wr_header.flag=0x01;
			wr_header.ID=0;
			Write_Header(Flash_SPI,wr_header,0 );
			AD_i=0;
			ADFlag=0;
			SN2=0;
			SN1=0;
			LED_OFF(LED_RED);
		}else if(StopFlag && AD_i==0){
			StopFlag=0;
			AD_i=0;
			ADFlag=0;
			SN2=0;
			SN1=0;
			LED_OFF(LED_RED);
		}
		
		if(FindFlag==1 && timer_50ms_tick /*&& isRec==0*/){
					timer_50ms_tick = 0;
					ADS(ADS1248_SPI,RxData);
					uint8_t btsAD_Log[]={0x01,CMD_AD_FIND_RESPOND,0x00,0x00,
															RxData[0]>>8&0xFF,RxData[0]&0xFF,
															RxData[1]>>8&0xFF,RxData[1]&0xFF,
															RxData[2]>>8&0xFF,RxData[2]&0xFF,
															RxData[3]>>8&0xFF,RxData[3]&0xFF,
															RxData[4]>>8&0xFF,RxData[4]&0xFF};
					ble_nus_send_string(&m_nus, btsAD_Log, 14);
															
					isRec=1;
			}
		if(isvalve==1 && valve_i==1){
			nrf_gpio_pin_clear(valve);
			nrf_delay_us(50000);
			nrf_gpio_pin_set(valve);
		}
			
	/*
	//=====================================
	//Find operate in chip not in computer.
	//=====================================
		if(FindFlag!=0){
			if(FindFlag==1){
				MX25_RD(Flash_SPI,0x00,0x00,4,bts_get);
				AD_A=bts_get[0]<<8 | bts_get[1];
				AD_dA=bts_get[2]<<8 | bts_get[3];
				FindFlag=9;
			}
			if(FindFlag==2){
				MX25_RD(Flash_SPI,0x00,0x04,4,bts_get);
				AD_A=bts_get[0]<<8 | bts_get[1];
				AD_dA=bts_get[2]<<8 | bts_get[3];
				FindFlag=9;
			}
			if(FindFlag==3){
				MX25_RD(Flash_SPI,0x00,0x08,4,bts_get);
				AD_A=bts_get[0]<<8 | bts_get[1];
				AD_dA=bts_get[2]<<8 | bts_get[3];
				FindFlag=9;
			}
			
			if(FindFlag==9){
				ADS(ADS1248_SPI,RxData);
				int pp=0;
				for (pp=0;pp<5;pp++){
					ADbase[pp]=RxData[pp];
				}
				FindFlag=8;
			}

			if(FindFlag==8){
				for(ADbuff_i=0;ADbuff_i<=9;ADbuff_i++){
					ADS(ADS1248_SPI,RxData);
					int p=0;
					for(p=0;p<5;p++){
						ADbuff[ADbuff_i][p]=RxData[p]-ADbase[p];
						ADall+=ADbuff[ADbuff_i][p];
					}
				}
				FindFlag=7; 
			}

			if(  ADall/50-AD_A<AD_dA && ADall/50-AD_A>-AD_dA ){
				FindFlag=0;
				//ADFlag=1;  //!!!!
				uint8_t bts_find_success[]={0x01,CMD_AD_FIND,0x00,0x00,0xAA};
				while(ble_nus_send_string(&m_nus, bts_find_success, 5)){}
			}else{
				int p=0;
				ADS(ADS1248_SPI,RxData);
				for(p=0;p<5;p++){
					ADall-=ADbuff[ADdel][p];
					ADbuff[ADdel][p]=RxData[p]-ADbase[p];
					ADall+=ADbuff[ADdel][p];
				}
				
				if(ADdel>=10) ADdel=0;
				
				ADdel++;
			}

			if(ADtimeout>334000){
				FindFlag=0;
				ADtimeout=0;
				ADbuff_i=0;
				ADall=0;
				ADdel=0;
				uint8_t bts_find_timeout[]={0x01,CMD_AD_FIND,0x00,0x00,0xBB};
				while(ble_nus_send_string(&m_nus, bts_find_timeout, 5)){}
			}

		}
*/
		
		if(EraseFlag){
			uint8_t btsErase[]={0x01,0x71};
			//ble_nus_send_string(&m_nus, btsErase, 2);
			MX25_EraseFlash(Flash_SPI);
			Flash_init(Flash_SPI);
			btsErase[1]=0x72;
			ble_nus_send_string(&m_nus, btsErase, 2);
			EraseFlag=0;
		}
		

		
		
		
	}  
	
}

