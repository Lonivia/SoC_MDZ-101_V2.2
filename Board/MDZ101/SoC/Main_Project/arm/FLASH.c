#include "ADS1248.h"
#include "nrf_gpio.h"
#include "common.h"
#include "spi_master.h"
#include "MDZ_Board.h"
#include "spi_master_config.h" 
#include "FLASH.h"

long int Write_Ptr;
long int Read_Ptr;
uint8_t Write_PATIENT_NO;
uint8_t Read_PATIENT_NO;

uint8_t Write_Protection=0;


/************根据busy位来判定flash当前是否正在执行page programming**************************/

char MX25_CheckStatusBusy(uint32_t *spi_base_address)
{
	unsigned char StatRgVal = 0;

	do
	{
		Flash_CE_LOW;			 
		Flash_Send_Byte(spi_base_address,0x05);					// ??????????
		StatRgVal = Flash_Get_Byte(spi_base_address); 			// ???????????
		Flash_CE_HIGH;
		
		ClearWdt();

	}
	while ( (StatRgVal & 0x01) == 0x01 );				// ????,??????
	return true;
}




/******************向flash的spi总线上写入数据*****************************/

void Flash_Send_Byte(uint32_t *spi_base_address,uint8_t cmd){

   uint8_t tx_cmd[]={cmd};
	
   uint8_t temp[]={0};
	
   spi_master_tx_rx(spi_base_address,1,tx_cmd,temp);


}

/***************在flash的spi总线上读取数据***********************/

uint8_t Flash_Get_Byte(uint32_t *spi_base_address){

	uint8_t tx[1]={0xFF};

	uint8_t rx[1];
	
  spi_master_tx_rx(spi_base_address,1,tx,rx);

	return rx[0];
}

/************flash写使能*************************/

void FLASH_WR_Enable(uint32_t *spi_base_address)	// ????
{
	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x06);							// ??????????
	Flash_CE_HIGH;

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x01);							// ??????????
	Flash_Send_Byte(spi_base_address,0);								// ?0BPx?,?Flash?????? 
	Flash_CE_HIGH;

	MX25_CheckStatusBusy(spi_base_address);
}


/***************读取flash中指定地址的数据***********************/

unsigned char MX25_RD(uint32_t *spi_base_address,uint16_t PageID, uint8_t PageAddr, uint8_t NByte, uint8_t *RcvBufPt )
{
	unsigned int i = 0;
	
	if((PageID > FLASH_PAGE_TOTAL_NUM)||(PageAddr > FLASH_PAGE_SIZE)||(NByte == 0))   return (false); //??????

	MX25_CheckStatusBusy(spi_base_address);

	Flash_CE_LOW;
/*	if( Send_Byte(spi_base_address,0x0B) == false )						// ?????
	{
		return false;
	}
	*/
	Flash_Send_Byte(spi_base_address,0X03);
	Flash_Send_Byte(spi_base_address,(PageID & 0xFFFF) >> 8);	// ??????:????3?????
	Flash_Send_Byte(spi_base_address,PageID & 0xFF);
	Flash_Send_Byte(spi_base_address,PageAddr & 0xFF);

	for(i = 0; i < NByte; i++)
	{
	    RcvBufPt[i] = Flash_Get_Byte(spi_base_address);
	}
	Flash_CE_HIGH;
	return true;
}


/***************向flash中的指定地址中写入数据***************************/

unsigned char MX25_WR(uint32_t *spi_base_address,uint16_t PageID, uint8_t PageAddr, const uint8_t *SndBufPt, uint8_t NByte)
{
	unsigned int i = 0;
	if((PageID > FLASH_PAGE_TOTAL_NUM)||(PageAddr > FLASH_PAGE_SIZE)||(NByte == 0))   return (false); //??????

	MX25_CheckStatusBusy(spi_base_address);

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x06);						//写使能
	Flash_CE_HIGH;			

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x02);						// 写指令
	Flash_Send_Byte(spi_base_address,(PageID & 0xFFFF) >> 8);	// 写地址
	Flash_Send_Byte(spi_base_address,PageID & 0xFF);
	Flash_Send_Byte(spi_base_address,(PageAddr) & 0xFF);

	for(i = 0; i < NByte; i++)
	{
		Flash_Send_Byte(spi_base_address,SndBufPt[i]);				// 写字节
	}
	Flash_CE_HIGH;
	MX25_CheckStatusBusy(spi_base_address);

	return true;	
}

/*********************清flash中的一个sector*************************/

bool MX25_EraseSector(uint32_t *spi_base_address,uint8_t Sector )		// 60~300ms
{
	unsigned int  SecnHdAddr = 0;

	if(Sector > SEC_MAX) return false;
	
	//TRACE1( "\nErase Sector: %u", Sector);

	MX25_CheckStatusBusy(spi_base_address);

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x06);						// ???????(???????,WEL????,???????)
	Flash_CE_HIGH;			

	SecnHdAddr = SEC_SIZE * Sector;			// ?????????
	Flash_CE_LOW;	
    Flash_Send_Byte(spi_base_address,0x20);							// ????????
	  Flash_Send_Byte(spi_base_address,(SecnHdAddr & 0xFFFFFF) >> 16); // ??3????????
    Flash_Send_Byte(spi_base_address,(SecnHdAddr & 0xFFFF) >> 8);
   	Flash_Send_Byte(spi_base_address,SecnHdAddr & 0xFF);
  	Flash_CE_HIGH;	
	
	MX25_CheckStatusBusy(spi_base_address);

	return true;
}

/*********************清flash中的一个block*************************/

bool  MX25_EraseBlock(uint32_t *spi_base_address,uint8_t Block)	// 0.7~2?
{
	unsigned int BlockHdAddr = 0;

	if(Block > BLOCK_MAX) return false;

	MX25_CheckStatusBusy(spi_base_address);

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x06);						// ???????(???????,WEL????,???????)
	Flash_CE_HIGH;			

	BlockHdAddr =Block * BLOCK_SIZE;			// ?????????
	Flash_CE_LOW;	
    Flash_Send_Byte(spi_base_address,0xD8);							// ????????
	  Flash_Send_Byte(spi_base_address,(BlockHdAddr & 0xFFFFFF) >> 16); // ??3????????
    Flash_Send_Byte(spi_base_address,(BlockHdAddr & 0xFFFF) >> 8);
   	Flash_Send_Byte(spi_base_address,BlockHdAddr & 0xFF);
  	Flash_CE_HIGH;			

	MX25_CheckStatusBusy(spi_base_address);	

	return true;
}

/*********************清整个flash芯片************************/

bool  MX25_EraseFlash(uint32_t *spi_base_address)
{
	MX25_CheckStatusBusy(spi_base_address);

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x06);						// ???????(???????,WEL????,???????)
	Flash_CE_HIGH;			

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x60);							// ????????(60h or C7h)
	Flash_CE_HIGH;	
	
	MX25_CheckStatusBusy(spi_base_address);

	return true;
}

/*********************清flash中的sector1到sector2************************/

unsigned char  MX25_Erase_Sec1ToSec2(uint32_t *spi_base_address,uint32_t Sec1, uint32_t Sec2)
{
	unsigned int temp2;
    unsigned int SecnHdAddr = 0;	  			
	unsigned int No_SecsToEr = 0;				    // ????????
	unsigned int CurSecToEr = 0;	  				// ?????????
	
	/*  ?????? */
	if( Sec1 >= SEC_MAX || Sec2 >= SEC_MAX )	
	{
		return false;	
	}
   	
	MX25_CheckStatusBusy(spi_base_address);

	Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0x06);								// ???????(???????,WEL????,???????)
	Flash_CE_HIGH;			

	/* ???????????????????,???????? */
	if (Sec1 > Sec2)
	{
	   temp2 = Sec1;
	   Sec1  = Sec2;
	   Sec2  = temp2;
	} 
	/* ??????????????? */
	if (Sec1 == Sec2)	
	{
	    SecnHdAddr = SEC_SIZE * Sec1;				// ?????????
	    Flash_CE_LOW;	
    	Flash_Send_Byte(spi_base_address,0x20);							// ????????
	    Flash_Send_Byte(spi_base_address,(SecnHdAddr & 0xFFFFFF) >> 16); // ??3????????
   		Flash_Send_Byte(spi_base_address,(SecnHdAddr & 0xFFFF) >> 8);
   		Flash_Send_Byte(spi_base_address,SecnHdAddr & 0xFF);
  		Flash_CE_HIGH;		
		MX25_CheckStatusBusy(spi_base_address);		
		return true;			
	}
	
     /* ------------  ??????????????????????? --------------*/		
	if (Sec2 - Sec1 == SEC_MAX)	
	{
		Flash_CE_LOW;			
		Flash_Send_Byte(spi_base_address,0x60);							// ????????(60h or C7h)
		Flash_CE_HIGH;			
		MX25_CheckStatusBusy(spi_base_address);
   		return true;
	}
	
	No_SecsToEr = Sec2 - Sec1 +1;					// ??????????
	CurSecToEr  = Sec1;								// ?????????
	
	/* ????????????,???16?????? */
	while (No_SecsToEr >= 16)
	{
		Flash_CE_LOW;			
		Flash_Send_Byte(spi_base_address,0x06);							// ???????(???????,WEL????,???????)
		Flash_CE_HIGH;			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// ?????????
	    Flash_CE_LOW;	
	    Flash_Send_Byte(spi_base_address,0xD8);							// ??64KB?????
	    Flash_Send_Byte(spi_base_address,((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
   		Flash_Send_Byte(spi_base_address,((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_Send_Byte(spi_base_address,SecnHdAddr & 0xFF);
  		Flash_CE_HIGH;			
			MX25_CheckStatusBusy(spi_base_address);
			CurSecToEr  += 16;							// ?????16????,??????????????
			No_SecsToEr -=  16;							// ?????????????
	}
	/* ????????????,???8?????? */
	while (No_SecsToEr >= 8)
	{
		Flash_CE_LOW;			
		Flash_Send_Byte(spi_base_address,0x06);							// ???????(???????,WEL????,???????)
		Flash_CE_HIGH;			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// ?????????
	    Flash_CE_LOW;	
	    Flash_Send_Byte(spi_base_address,0x52);							// ??32KB????
	    Flash_Send_Byte(spi_base_address,((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
   		Flash_Send_Byte(spi_base_address,((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_Send_Byte(spi_base_address,SecnHdAddr & 0xFF);
  		Flash_CE_HIGH;			
		MX25_CheckStatusBusy(spi_base_address);
		CurSecToEr  += 8;
		No_SecsToEr -=  8;
	}
	/* ??????????????? */
	while (No_SecsToEr >= 1)
	{
		Flash_CE_LOW;			
		Flash_Send_Byte(spi_base_address,0x06);							// ???????(???????,WEL????,???????)
		Flash_CE_HIGH;			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// ?????????
	    Flash_CE_LOW;	
    	Flash_Send_Byte(spi_base_address,0x20);							// ????????
	    Flash_Send_Byte(spi_base_address,((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
   		Flash_Send_Byte(spi_base_address,((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_Send_Byte(spi_base_address,SecnHdAddr & 0xFF);
  		Flash_CE_HIGH;			
		MX25_CheckStatusBusy(spi_base_address);
		CurSecToEr  += 1;
		No_SecsToEr -=  1;
	}
   
	return true;
}



/********************flash初始化************************/

void Flash_init(uint32_t *spi_base_address){
	
	nrf_gpio_cfg_output(SPI_PSELSCK1);
	nrf_gpio_cfg_output(SPI_PSELSS1);
	nrf_gpio_cfg_output(SPI_PSELMOSI1);
	nrf_gpio_cfg_input(SPI_PSELMISO1, NRF_GPIO_PIN_PULLDOWN);
 
	//MX25_EraseSector(spi_base_address,0);
	//MX25_EraseSector(spi_base_address,1);
	//MX25_EraseSector(spi_base_address,2);
	//!!!!erase 1 block every time.
	MX25_EraseBlock(spi_base_address,4);
	MX25_EraseBlock(spi_base_address,5);
	MX25_EraseBlock(spi_base_address,6);
	MX25_EraseBlock(spi_base_address,7);
	
	
  uint8_t Readedflag[1]={0};
	int i;	
	for(i=1;i<=15;i++){
		MX25_RD(spi_base_address,(i*0x40000)>>8, 0x00, 1, Readedflag);	
		if(Readedflag[0]==0xFF){
		   Write_PATIENT_NO=i;
		   Write_Ptr=1<<12;
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			Read_PATIENT_NO=i;
		   Read_Ptr=1<<12;
		}		
  else{
		continue;
	}		
		break;
	}
	/*
	for(i=1;i<Write_PATIENT_NO;i++){
	  MX25_RD(spi_base_address,(i*0x40000)>>8, 0x00, 1, Readedflag);
		if(Readedflag[0]==0x0F){
		
		   Read_PATIENT_NO=i;
		   Read_Ptr=1<<12;
		}	
		else{
	    continue;
		}
	   break;	
	}
	*/
	
	Write_PATIENT_NO=1;
	Write_Ptr=1<<12;
	Read_PATIENT_NO=1;
	Read_Ptr=1<<12;
	//only use first patient!!!
	
	
	
	if(i==Write_PATIENT_NO){
		
			Read_PATIENT_NO=Write_PATIENT_NO;
		  Read_Ptr=1<<12;

	}		
	

  if(Write_PATIENT_NO>=13){

		if(Read_PATIENT_NO==Write_PATIENT_NO){
			  
		/*	for(int i=1*4;i<Write_PATIENT_NO*4;i++){
			  
			   MX25_EraseBlock(spi_base_address,i);
			
			}*/
		MX25_EraseFlash(spi_base_address);
		Write_PATIENT_NO=1;	
		Read_PATIENT_NO=1;
		 Write_Protection=0;
		}
		else{
			
	  Write_Protection=1;
		
		}
	}
	
}

/*************************向flash中按协议写入数据包*****************************/

unsigned char Write_ADdata(uint32_t *spi_base_address,unsigned short data[5]){
	
 uint8_t Data[16];
	
	  Data[0]=0x0F;

	for(int i=1;i<6;i++){
	   Data[i]=0xFF;
	}
	for(int i=6;i<16;i+=2){
	    Data[i]=data[(i-6)/2]>>8;
		  Data[i+1]=data[(i-6)/2]&0xFF;
	}
	
	MX25_WR(spi_base_address,(Write_Ptr+(Write_PATIENT_NO*0x40000))>>8, (Write_Ptr)&0xFF, Data, 16);
	
	Write_Ptr+=sizeof(struct AD_Data);   

 return true;
	
}

/****************读取flash中的数据包，返回在data指针中***********************/

unsigned char Read_ADdata(uint32_t *spi_base_address,struct AD_Data *data){

  uint8_t basedata[16];
	
  uint8_t Readed[1]={0x01};
	
  MX25_RD(spi_base_address,(Read_Ptr+(Read_PATIENT_NO*0x40000))>>8,Read_Ptr&0xFF, 16, basedata);
 
	data[0].flag=basedata[0];
	
	data[0].reserved[0]=basedata[1];
  data[0].reserved[1]=basedata[2];
	data[0].reserved[2]=basedata[3];
	data[0].reserved[3]=basedata[4];
  data[0].reserved[4]=basedata[5];

	data[0].channel[0]=basedata[6]<<8 | basedata[7];
	data[0].channel[1]=basedata[8]<<8 | basedata[9];
	data[0].channel[2]=basedata[10]<<8 | basedata[11];
	data[0].channel[3]=basedata[12]<<8 | basedata[13];
	data[0].channel[4]=basedata[14]<<8 | basedata[15];

  MX25_WR(spi_base_address,(Read_Ptr+(Read_PATIENT_NO*0x40000))>>8, (Read_Ptr)&0xFF, Readed, 1);

  Read_Ptr+=sizeof(struct AD_Data);

  return true;
}

void Write_Header(uint32_t *spi_base_address,struct Header header,uint8_t wr_select){

uint8_t WR_flag[5];	
	
WR_flag[0]=header.flag;
	
WR_flag[1]=header.ID>>24;

WR_flag[2]=(header.ID>>16)&0xFF;

WR_flag[3]=(header.ID>>8)&0xFF;

WR_flag[4]=header.ID&0xFF;

if(wr_select==0){
	
MX25_WR(spi_base_address,(Write_PATIENT_NO*0x40000)>>8, 0x00,WR_flag, 5);

}
if(wr_select==1){

MX25_WR(spi_base_address,(Read_PATIENT_NO*0x40000)>>8, 0x00,WR_flag, 5);

}

}
void Read_Header(uint32_t *spi_base_address,struct Header *header){

 uint8_t WR_flag[5]={0}; 
 
 MX25_RD(spi_base_address,(Read_PATIENT_NO*0x40000)>>8, 0x00,5,WR_flag);
	
 header[0].flag=WR_flag[0];
  
 header[0].ID=(WR_flag[1]<<24) | (WR_flag[2]<<16) | (WR_flag[3]<<8) | (WR_flag[4]);
 
}

void Flash_Sleep(uint32_t *spi_base_address){

  Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0xB9);							
	Flash_CE_HIGH;		

}
void Flash_Wakeup(uint32_t *spi_base_address){

  Flash_CE_LOW;			
	Flash_Send_Byte(spi_base_address,0xAB);							
	Flash_CE_HIGH;



}

