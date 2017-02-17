#include <stdint.h>
#include <string.h>
#include "nrf_delay.h"
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
#include "MDZ_Board.h"
#include "BLEhandle.h"
#include "CRC16.h"
#include "Flash.h"
#include "spi_master.h"
#include "ADS1248.h"



ble_nus_t                        m_nus; 

uint8_t bts_test[]={0x12,0x34,0x56,0x78};

int Send_i=0;
int AD_i=0;
int isUSB=0;
int isFULL=0;

uint8_t isvalve=0;

uint8_t isRec=0;
uint8_t isRec2=0;

void BLE_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length){
	
		//Start receive data
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_RECEIVE_START){
			//Flash_init(Flash_SPI);
			isRec2=0;
			SendFlag=1;
			Send_i=0;
		}
		
		//Start AD collecton
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_AD_START){
			if(p_data[2]==0x01){
				Flash_init(Flash_SPI);
			}
			SendFlag=0;
			ADFlag=1;
			AD_i=0;
			pumpflag=0;
			isvalve=0;
			nrf_gpio_pin_set(valve);
		}
		
		//Start AD find
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_AD_FIND){
			if(p_data[4]==0x00){
				pumpflag=1;
				nrf_gpio_pin_set(valve);
				
				uint8_t bts_handshake[]={0x01,0x99,0x00,0x00,p_data[4]};
			  while(ble_nus_send_string(&m_nus, bts_handshake, 5)){}
			}else if(p_data[4]==0x01){
				
				//nrf_gpio_pin_clear(valve);
				isvalve=1;
				uint8_t bts_handshake[]={0x01,0x99,0x00,0x00,p_data[4]};
			  while(ble_nus_send_string(&m_nus, bts_handshake, 5)){}
			}else if(p_data[4]==0x02){

				//nrf_gpio_pin_clear(valve);
				isvalve=1;
				uint8_t bts_handshake[]={0x01,0x99,0x00,0x00,p_data[4]};
			  while(ble_nus_send_string(&m_nus, bts_handshake, 5)){}
			}else{
				uint8_t bts_handshake[]={0x01,0x99,0x00,0x00,0x99};
			  while(ble_nus_send_string(&m_nus, bts_handshake, 5)){}
			}
			//pumpflag=1;
			//valveflag=1;
				SendFlag=0;
				ADFlag=0;
				StopFlag=1;
				FindFlag=1;
		}
		
		if(p_data[0]==FLAG_CMD && p_data[1]==0x13){
			isRec=0;
		}
		if(p_data[0]==FLAG_CMD && p_data[1]==0x14){
			isRec2=0;
		}
		
		
		if(p_data[0]==FLAG_CMD && p_data[1]==0x09){
			nrf_gpio_pin_clear(valve);
			uint8_t bts_valve[]={0x01,0x10};
			  while(ble_nus_send_string(&m_nus, bts_valve, 2)){}
		}
		
		
		//Stop AD find and collection
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_AD_STOP){
			//nrf_gpio_pin_clear(valve);
			//nrf_delay_us(100000);
			//nrf_gpio_pin_set(valve);
			nrf_gpio_pin_clear(pump);
			pumpflag=0;
			isvalve=0;
				FindFlag=0;
				ADFlag=0;
				uint8_t bts_stop_res[]={0x01,CMD_AD_STOP_RESPOND};
				ble_nus_send_string(&m_nus, bts_stop_res, 2);
				StopFlag=1;
		}

		/*
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_AD_FIND){
			
			SendFlag=0;
			ADFlag=0;
			FindFlag=p_data[4];
			uint8_t bts_find[]={0x01,CMD_AD_FIND,0x00,0x00,p_data[4]};
			while(ble_nus_send_string(&m_nus, bts_find, 5)){}

		}
		*/
		
		//Flash Erase
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_FLASH_ERASE){
			EraseFlag=1;
		}
		
		//Read bat and usb
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_READ_BAT){
			uint32_t adc_data=adc_read_usb();
			uint8_t BAT=0;
			if((uint8_t)adc_data>=0xC8){
				BAT=100;
			}else if((uint8_t)adc_data>0xC6 && (uint8_t)adc_data<0xC8){
				BAT=90;
			}else if((uint8_t)adc_data>0xC4 && (uint8_t)adc_data<=0xC6){
				BAT=80;
			}else if((uint8_t)adc_data>0xC2 && (uint8_t)adc_data<=0xC4){
				BAT=70;
			}else if((uint8_t)adc_data>0xC0 && (uint8_t)adc_data<=0xC2){
				BAT=60;
			}else if((uint8_t)adc_data>0xBB && (uint8_t)adc_data<=0xC0){
				BAT=50;
			}else if((uint8_t)adc_data>0xB6 && (uint8_t)adc_data<=0xBB){
				BAT=40;
			}else if((uint8_t)adc_data>0xB0 && (uint8_t)adc_data<=0xB6){
				BAT=30;
			}else if((uint8_t)adc_data>0xB6 && (uint8_t)adc_data<=0xB4){
				BAT=20;
			}else if((uint8_t)adc_data>0xB4 && (uint8_t)adc_data<=0xAF){
				BAT=10;
			}else if((uint8_t)adc_data<=0xAF){
				BAT=0;
			}
			uint32_t adc_data2=adc_read_usb();
			//if((char)adc_data>0xCC)
			
			if( (uint8_t)adc_data2>=0xDD || (uint8_t)adc_data2<=0x04) 
	    {	
				isUSB=1;
	  	}else{
				isUSB=0;
			}
			if(isUSB==1 && BAT==100 && nrf_gpio_pin_read(CHRG)== 1){//pin init in key.c!!
				isFULL=1;
			}else{
				isFULL=0;
			}
			uint8_t bts_bat[]={0x01,CMD_READ_BAT_RESPOND,(uint8_t)adc_data2,(uint8_t)adc_data,BAT,isUSB,isFULL};
			ble_nus_send_string(&m_nus, bts_bat, 7);
				
			//adc_data=adc_read_usb();
			//uint8_t bts_usb[]={0x01,CMD_READ_BAT_RESPOND,(char)(adc_data>>24),(char)(adc_data>>16),(char)(adc_data>>8),(char)adc_data};
			//while(ble_nus_send_string(&m_nus, bts_usb, 6)){}
		}
		
		//Read SN and Ver
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_SN_VER){
			uint8_t bts_sn[]={0x01,0x34,0x00,0x00,0x96,0x00};
			while(ble_nus_send_string(&m_nus, bts_sn, 6)){}
			uint8_t bts_ver[]={0x00,0x34,0x00,0x00,0x00,0x21};
			while(ble_nus_send_string(&m_nus, bts_ver, 6)){}
		}
		
		//Handshake
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_HANDSHAKE){
			uint8_t bts_handshake[]={0x01,CMD_HANDSHAKE_RESPOND,0x00,0x00,p_data[4],p_data[5]};
			while(ble_nus_send_string(&m_nus, bts_handshake, 6)){}
		}

		/*
		if(p_data[0]==FLAG_CMD && p_data[1]==CMD_CONFIG){
			
			uint8_t bts_setok[]={0x01,0x56,0x00,0x00,0x00};
			
			if(p_data[4]==0x01){
				uint8_t bts_set01[]={p_data[5],p_data[6],p_data[7],p_data[8]};
				if ( MX25_WR(Flash_SPI,0x00,0x00,bts_set01,4) ){
					bts_setok[4]=0x01;
					while(ble_nus_send_string(&m_nus, bts_setok, 5)){}
				}else{
					bts_setok[1]=CMD_CONFIG_FAIL;
					while(ble_nus_send_string(&m_nus, bts_setok, 5)){}
				}
			}
			if(p_data[4]==0x02){
				uint8_t bts_set02[]={p_data[5],p_data[6],p_data[7],p_data[8]};
				if ( MX25_WR(Flash_SPI,0x00,0x04,bts_set02,4) ){
					bts_setok[4]=0x02;
					while(ble_nus_send_string(&m_nus, bts_setok, 5)){}
				}else{
					bts_setok[1]=CMD_CONFIG_FAIL;
					while(ble_nus_send_string(&m_nus, bts_setok, 5)){}
				}
			}
			if(p_data[4]==0x03){
				uint8_t bts_set03[]={p_data[5],p_data[6],p_data[7],p_data[8]};
				if ( MX25_WR(Flash_SPI,0x00,0x08,bts_set03,4) ){
					bts_setok[4]=0x03;
					while(ble_nus_send_string(&m_nus, bts_setok, 5)){}
				}else{
					bts_setok[1]=CMD_CONFIG_FAIL;
					while(ble_nus_send_string(&m_nus, bts_setok, 5)){}
				}
			}
		
		}
		*/
		
}



