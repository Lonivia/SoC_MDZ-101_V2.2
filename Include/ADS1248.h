
#ifndef _ADS1248_H_
#define _ADS1248_H_

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"

#define   ADS1248_CMD_WAKEUP    0x00
#define   ADS1248_CMD_SLEEP     0x02
#define   ADS1248_CMD_SYNC      0x04
#define   ADS1248_CMD_RESET     0x06
#define   ADS1248_CMD_NOP       0xff
#define   ADS1248_CMD_RDATA     0x12
#define   ADS1248_CMD_RDATAC    0x14
#define   ADS1248_CMD_SDATAC    0x16
#define   ADS1248_CMD_RREG      0x20
#define   ADS1248_CMD_WREG      0x40

#define   ADS1248_REG_MUX0    0x00
#define   ADS1248_REG_VBIAS   0x01
#define   ADS1248_REG_MUX1    0x02
#define   ADS1248_REG_SYS0    0x03
#define   ADS1248_REG_CFC0    0x04
#define   ADS1248_REG_CFC1    0x05
#define   ADS1248_REG_CFC2    0x06
#define   ADS1248_REG_FSC0    0x07
#define   ADS1248_REG_FSC1    0x08
#define   ADS1248_REG_FSC2    0x09
#define   ADS1248_REG_IDAC0   0x0A
#define   ADS1248_REG_IDAC1   0x0B
#define   ADS1248_REG_GPIOCFG  0x0C
#define   ADS1248_REG_GPIODIR  0x0D
#define   ADS1248_REG_GPIODAT  0x0E

/***ADS1248***/
void ADS1248WREG(uint32_t *spi_base_address,unsigned char regaddr,uint8_t databyte);
unsigned short ADS1248RRDATA(uint32_t *spi_base_address);
void InitADS1248(uint32_t *spi_base_address);
uint8_t ADS1248RREG(uint32_t *spi_base_address,unsigned char regaddr);
void ADS(uint32_t *spi_base_address,unsigned short *ReadData);
void ADS1248WCMD(uint32_t *spi_base_address,uint8_t cmd);

void AD_Sleep(uint32_t *spi_base_address);
void AD_Wakeup(uint32_t *spi_base_address);
void AD_Reset(uint32_t *spi_base_address);

#endif 
