#ifndef _STRUCT_H
#define _STRUCT_H


#define  MDZ_CMD_WRITE_CONFIG		0x22	
#define  MDZ_CMD_WRITE_STATUS		0x90	
#define  MDZ_CMD_READ_RECORD			0x9A	
#define  MDZ_CMD_READ_LOG		0x9B	

#define MDZ_DATA_LEN = 3*5 ;

//************************************************************************
typedef struct
{
	unsigned char Header;	
	unsigned char BLEAddr;	
	unsigned char DataLen;		 //only data length 
	unsigned char CRC;  //head+data:CRC
	
} MDZ_SEND_HEAD ;

typedef struct{
	
	unsigned char DataFlag
	unsigned char Data[MDZ_DATA_LEN];	
	
} *MDZ_SEND_DATA;

#endif
