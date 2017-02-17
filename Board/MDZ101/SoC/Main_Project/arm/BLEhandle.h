
#ifndef  _BLEhandle_H_
#define  _BLEhandle_H_

#include <stdint.h>
#include <string.h>
//Flag Define

#define FLAG_DATA_WATCH 0x02
#define FLAG_DATA_BELT 0x03
#define FLAG_CMD 0x01
#define FLAG_TEXT 0x00

//CMD Define
#define CMD_HANDSHAKE 0x46
#define CMD_HANDSHAKE_RESPOND 0x47

#define CMD_RECEIVE_START 0x43
#define CMD_RECEIVE_RESPOND_SUCCESS 0x44
#define CMD_RECEIVE_RESPOND_FINISH 0x45

#define CMD_AD_FIND 0x54
#define CMD_AD_FIND_RESPOND 0x55
#define CMD_AD_START 0x36
#define CMD_AD_START_SUCCESS 0x37
#define CMD_AD_FINISH 0x42
#define CMD_AD_STOP 0x57
#define CMD_AD_STOP_RESPOND 0X58

#define CMD_READ_BAT 0x51
#define CMD_READ_BAT_RESPOND 0x52

#define CMD_CONFIG 0x54
#define CMD_CONFIG_FAIL 0X55

#define CMD_FLASH_ERASE 0x71
#define CMD_FLASH_ERASE_RESPOND 0x72

#define CMD_CHECK_CRC16 0x99

#define CMD_SN_VER 0x33

//FLAG Define
extern uint8_t SendFlag;
extern uint8_t ADFlag;
extern uint8_t EraseFlag;
extern uint8_t FindFlag;
extern uint8_t StopFlag;




#endif
