#ifndef _RS485_H
#define _RS485_H

//////////////////////////////////////////////////////////////////////////
// hardware definitions
#define NRF_BAND0						9600
#define TIMER2INI						(65536L-SYSCLOCK/NRF_BAND0/32)
#define TH2INI							(TIMER2INI>>8)
#define TL2INI							TIMER2INI

//************************************************************************
#define RS485_REC_HEADER			0x0A
#define RS485_SEND_HEADER			0x0B

//************************************************************************
#define MAX_SEND_TAG_NUM			30		// 每次回复的ACK中最多包含的记录数量

//////////////////////////////////////////////////////////////////////////
// function definitions
extern void RS485_SerialIni(void);
extern void RS485_HandleMessage( void );
extern void RS485_SendCardAckInfo( unsigned char ucMode );
extern void RS485_AutoSendCardAckInfo( void );
extern void RS485_Send( unsigned char * pSend, unsigned char Len );

#endif

