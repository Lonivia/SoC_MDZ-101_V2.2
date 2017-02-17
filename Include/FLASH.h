#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"

#define FLASH_PAGE_TOTAL_NUM 16384
#define FLASH_PAGE_SIZE 0xFF
#define SEC_MAX 0x0F
#define SEC_SIZE 0x01<<12
#define BLOCK_MAX 63
#define BLOCK_SIZE (0x01)<<16


struct AD_Data{

  unsigned char flag;

	uint8_t reserved[5]; 
	
  short int channel[5];

};

struct Header{

	unsigned char flag;

	uint32_t ID;
	
};



void Flash_Send_Byte(uint32_t *spi_base_adress,uint8_t cmd);	
char Mx25_CheckStatusBusy( uint32_t *spi_base_adress);
uint8_t Flash_Get_Byte(uint32_t *spi_base_adress);
unsigned char MX25_RD(uint32_t *spi_base_adress,uint16_t PageID, uint8_t PageAddr, uint8_t NByte, uint8_t *RcvBufPt );
unsigned char MX25_WR(uint32_t *spi_base_adress,uint16_t PageID, uint8_t PageAddr, const uint8_t *SndBufPt, uint8_t NByte);
unsigned char  MX25_Erase_Sec1ToSec2(uint32_t *spi_base_adress,uint32_t Sec1, uint32_t Sec2);
bool  MX25_EraseFlash(uint32_t *spi_base_adress);
bool  MX25_EraseBlock(uint32_t *spi_base_adress,uint8_t Block);
bool MX25_EraseSector(uint32_t *spi_base_adress,uint8_t Sector );
void Flash_init(uint32_t *spi_base_address);


unsigned char Write_ADdata(uint32_t *spi_base_address,unsigned short data[5]);
unsigned char Read_ADdata(uint32_t *spi_base_address,struct AD_Data *data);

void Write_Header(uint32_t *spi_base_address,struct Header header,uint8_t wr_select);
void Read_Header(uint32_t *spi_base_address,struct Header *header);

void Flash_Sleep(uint32_t *spi_base_address);
void Flash_Wakeup(uint32_t *spi_base_address);

