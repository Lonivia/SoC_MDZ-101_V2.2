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
//#include "BLEhandle.h"
#include "nrf_delay.h"


int Timer_KEY=0;
int Timer_LED=0;
int Key_Count=0;
int Long_press=0;

int startBLE=0;
int enableBLE=1;

ble_nus_t * p_nus;
uint8_t * p_data;
uint16_t length;
	
uint8_t SendFlag=0;
uint8_t ADFlag=0;
uint8_t EraseFlag=0;
uint8_t FindFlag=0;
uint8_t StopFlag=0;

uint8_t BLE_flag=FLAG_DATA_WATCH;

uint8_t SN1=0x00;
uint8_t SN2=0x00;

struct AD_Data readdata[1]={0};

//extern int Send_i;
//extern int AD_i;
int Find_i=0;

unsigned short RxData[5]={0x1234,0x5678,0x9000,0xABCD,0xCDEF};

int AD_LED=0;

uint8_t bts_get[4];
int16_t ADbuff[10][5];
int ADbuff_i=0;
int ADall=0;
int AD_A=0;
int AD_dA=0;
uint8_t ADdel=0;
int ADtimeout=0;

int16_t ADbase[5];

