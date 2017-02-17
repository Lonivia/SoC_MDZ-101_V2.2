
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

#include "CRC16.h"
#include "MDZ_Board.h"

unsigned short const m_uiCRC16Tab[16] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};


unsigned int GetQuickCRC16(unsigned int uiCRC16, unsigned char * pStart, unsigned int uiLen)
{
	unsigned char ucHigh4Bits;

	while (uiLen-- != 0)
	{
		ucHigh4Bits = ((unsigned char)(uiCRC16 / 256)) / 16; 
		uiCRC16 <<= 4; 


		uiCRC16 ^= m_uiCRC16Tab[ucHigh4Bits ^ (*pStart / 16)];

		ucHigh4Bits = ((unsigned char)(uiCRC16 / 256)) / 16; 
		uiCRC16 <<= 4; 


		uiCRC16 ^= m_uiCRC16Tab[ucHigh4Bits ^ (*pStart &0x0f)];

		pStart++;
	}
	return uiCRC16;
} 


unsigned int crc_register=0;
unsigned int GetFlashCRC16(uint8_t data){
	
		unsigned char crc_upper=0;     
		

    crc_upper = ((unsigned char)(crc_register/256))/16;          
		crc_register <<= 4;              
		crc_register ^= m_uiCRC16Tab[crc_upper^(data/16)]; 
      
		crc_upper = ((unsigned char)(crc_register/256))/16; 
		crc_register <<= 4;   
    crc_register ^= m_uiCRC16Tab[crc_upper^(data&0x0F)]; 

		return crc_register;
}




