/*********************************Copyright (c)****************************************
***************************************************************************************
**	----------------------------------File Info----------------------------------------
**	File    Name:    Flash.h
**  Descriptions:	 包含所有对NOR Flash MX25L32的详细操作函数
**************************************************************************************/
#ifndef  _FLASH_H_
#define  _FLASH_H_


/* Macro Define ---------------------------------------------------------------------*/

// Every Page has 256 Byte,Every Sector 4K Byte.
#define	 FLASH_PAGE_SIZE                (unsigned int)256
#define  PERSECT_PAGE_NUM               (unsigned int)16
#define  FLASH_SECT_TOTAL_NUM			(unsigned int)1024	// The Flash has Sector Num
#define  FLASH_PAGE_TOTAL_NUM			(unsigned int)16384

// Transfer the page address to Phsical Address, or In Reverse
#define  ADDRtoPAGE(a)					(unsigned int)((unsigned int)(a) >> 8)
#define  ADDRinPAGE(a)					(unsigned int)((unsigned int)(a) & 0xFF)
#define  SECTtoPAGE(a)		            (unsigned int)((unsigned int)(a) << 4)
#define  SECTtoADDR(a)					(unsigned int)((unsigned int)(a) << 12)

/* ------------------------------ Whole Flash Allocation ----------------------------*/
//#define  FLASH_SECT_PARAMETER_NUM       (unsigned int)16      // PARAMETER Flash  16个扇区(1 Block) 
//#define  FLASH_SECT_CONFIG_NUM			(unsigned int)32      // Config Flash     32个扇区(2 Blocks)
#define  FLASH_SECT_DATA_NUM			(unsigned int)240     // Data Flash       240个扇区(15 Blocks)

// 频繁更新的数据区,起始于0号扇区,占用16个扇区(即1个Block)
//#define  FLASH_SECT_PARAMETER_FIRST     (unsigned int)0
//#define  FLASH_SECT_PARAMETER_LAST      (unsigned int)(FLASH_SECT_PARAMETER_FIRST + FLASH_SECT_PARAMETER_NUM - 1)

// 配置区,起始于16号扇区,占用32个扇区(即2个Blocks)
//#define  FLASH_ERASESECT_CONFIG_NO		(unsigned int)(FLASH_SECT_PARAMETER_LAST + 1)
//#define  FLASH_ERASESECT_CONFIG_ADDR	SECTtoADDR(FLASH_ERASESECT_CONFIG_NO)

//#define  FLASH_SECT_CONFIG_FIRST        (unsigned int)(FLASH_ERASESECT_CONFIG_NO + 1)
//#define  FLASH_SECT_CONFIG_LAST         (unsigned int)(FLASH_SECT_CONFIG_FIRST + FLASH_SECT_CONFIG_NUM - 2)

// Data区，起始于0号扇区,占用160个扇区(即10个Blocks)
#define  FLASH_ERASESECT_DATA_NO		(unsigned int)0;//(FLASH_SECT_CONFIG_LAST + 1)
#define  FLASH_ERASESECT_DATA_ADDR		SECTtoADDR(FLASH_ERASESECT_DATA_NO)

#define  FLASH_SECT_DATA_FIRST          (unsigned int)(FLASH_ERASESECT_DATA_NO + 1)
#define  FLASH_SECT_DATA_LAST           (unsigned int)(FLASH_SECT_DATA_FIRST + FLASH_SECT_DATA_NUM -2)
#define  FLASH_DATA_LAST_PAGE			(unsigned int)(SECTtoADDR(FLASH_SECT_DATA_LAST)|0xF00)

//*****************************************************************************************************************
#define  FLASH_SECT_LOCAL_DATA_NUM			(unsigned int)96     // Data Flash      96个扇区(6 Blocks)
// Data区，起始于762号扇区,占用96个扇区(即6个Blocks)
#define  FLASH_ERASESECT_LOCAL_DATA_NO		(unsigned int)(762)
#define  FLASH_ERASESECT_LOCAL_DATA_ADDR 	SECTtoADDR(FLASH_ERASESECT_LOCAL_DATA_NO)

#define  FLASH_SECT_LOCAL_DATA_FIRST    (unsigned int)(FLASH_ERASESECT_LOCAL_DATA_NO + 1)
#define  FLASH_SECT_LOCAL_DATA_LAST     (unsigned int)(FLASH_SECT_LOCAL_DATA_FIRST + FLASH_SECT_LOCAL_DATA_NUM -2)
#define  FLASH_LOCAL_DATA_LAST_PAGE		(unsigned int)(SECTtoADDR(FLASH_SECT_LOCAL_DATA_LAST)|0xF00)

// ------------------------------ Firmware Flash Allocation ----------------------------
#define  FLASH_SECT_FIRMWARE_NUM		(unsigned int)126     // Firmware Flash   126个扇区(6 Blocks)

// 升级固件程序存储于FLASH的最后504K空间内，占126个扇区
#define  FLASH_SECT_FIRMWARE_FIRST     (unsigned int)(FLASH_SECT_TOTAL_NUM - FLASH_SECT_FIRMWARE_NUM)
#define  FLASH_SECT_FIRMWARE_LAST      (unsigned int)(FLASH_SECT_FIRMWARE_FIRST + FLASH_SECT_FIRMWARE_NUM - 1)

#define  FLASH_SECT_SIGN     			(unsigned int)(FLASH_SECT_FIRMWARE_FIRST - 1)		   


//#define  FLASH_PAGE_STATION_NUM			(unsigned int)(60*16)		// Station info, 16*16 stations

/* -------------------------------  Flash State ------------------------------------ */
// Write Flash State
#define  FLASH_EMPTY					0xFF
#define  FLASH_WRITING					0xFE
#define  FLASH_NORMAL					0xFC
#define  FLASH_TRANSMITED				0xF8
#define  FLASH_INVALID					0x00

// Erase Flash State
#define	 FLASH_STATE_EMTPY				0xFF		// Init state
#define	 FLASH_STATE_ERASING			0xFC		// Erasing the page one by one
#define	 FLASH_STATE_NORMAL				0xF0		// Whole block erased normally

// Erase Sector state
#define	 FLASH_ERASESECT_EMTPY			0xFF		// ERASE BLOCK doesn't initialized
#define	 FLASH_ERASESECT_UPDATING		0xFC		// Updating the ERASE BLOCK
#define	 FLASH_ERASESECT_NORMAL			0xF0		// ERASE BLOCK is normal, whole flash erased
#define	 FLASH_ERASESECT_ERASING		0x00		// Erasing the ERASE BLOCK

// Erase Page Info Structure
typedef struct
{
	unsigned char  ucErasePageState;
	unsigned char  ucFlashState;
} ERASEPAGEINFO;

/* -------------------------  Config type definitions here ------------------------- */
// Config Type
#define  CONFIG_TYPE_COMMON				0x01		// 常用的设置信息
#define  CONFIG_TYPE_GASVALUE			0x02		// 油量纪录
#define  CONFIG_TYPE_LASTSTATION		0x03		// 最后站点纪录
#define  CONFIG_TYPE_MILEMETER			0x04		// 里程记录表
#define  CONFIG_TYPE_DIALOUT			0x05		// 拨出记录表
#define  CONFIG_TYPE_DIALIN				0x06		// 拨入记录表
#define  CONFIG_TYPE_REPLYMSG			0x07		// 回复消息设置
#define  CONFIG_TYPE_LINEINFO			0x08		// 线路信息
#define  CONFIG_TYPE_STATIONINFO		0x09		// 站点信息

// Flash Reply Message State
#define	 REPLY_MESSAGE_EMPTY			0xFF
#define	 REPLY_MESSAGE_VALID			0x55
#define	 REPLY_MESSAGE_INVALID			0x00

// 终端（整体或部分）格式化命令
#define  FLASH_FORMAT_ALL	            0
#define  FLASH_FORMAT_DATA	            1
#define  FLASH_FORMAT_CONFIG	        2

// Flash Record Definitions( 12 + 40 )
#define	 MAX_RECORD_LENGTH			    12

//////////////////// FLASH Save MileMeter Time ///////////////////
#define  FLASH_SAVE_MILEMETER_TIME		250		// unit : m	


// Flash Record Struct Define
typedef struct
{
	unsigned char	State;				//当前记录的状态
	unsigned char	MsgLen;				//数据长度,即Message的有效数据
	unsigned char	SequenceNo;			//序列号
	unsigned char	Reserved;			//
	unsigned char	Message[MAX_RECORD_LENGTH];
} FLASH_RECORD, * FLASH_RECORD_PTR;		// 16 Bytes

// Flash Record Length(64 Byte)
#define  FLASH_RECORD_LEN     sizeof(FLASH_RECORD)

// Flash Device Info
typedef struct
{
	unsigned int	RunningHour;
	unsigned int	PowerOnTimes;
	unsigned char	Status;
	unsigned char	Reserved[3];
	unsigned char   RegSerialNo[8];		// 注册序号
} FLASH_INFO, * FLASH_INFO_PTR;

/* Extern Define ---------------------------------------------------------------------*/

// 变量
extern FLASH_RECORD 	g_FlashReadRecord;
extern FLASH_RECORD 	g_FlashWriteRecord;
extern FLASH_INFO		m_FlashInfo;
extern FLASH_RECORD	g_FlashLocalReadRecord;			//读记录结构体

extern unsigned int m_pWritePtr;			//写指针
extern unsigned int m_pReadPtr;			//读记录的指针
extern unsigned int	m_ucPreReadPtr;		//预读指针

extern unsigned int	m_pWritePtr_local;				//写指针
extern unsigned int	m_pReadPtr_local;					//读记录的指针
extern unsigned int	m_ucPreReadPtr_local;				//预读记录的指针，记录从Flash中读出，但并未发送成功

// Flash Function  Definition
extern void FLASH_GetState( void );
extern bool ReadSinglePageSingleInstance( unsigned int uiSectID, unsigned char *p_Instance, unsigned char ucSize );
extern bool WriteSinglePageSingleInstance( unsigned int uiSectID, unsigned char *p_Instance, unsigned char ucSize );
extern bool WritePerSectorMultiInstance( unsigned int uiSectID, unsigned char *p_Instance, unsigned char ucSize);
extern bool ReadPerSectorMultiInstance( unsigned int uiSectID, unsigned char *p_Instance, unsigned char ucSize);
extern bool WriteSingleSectorMultiInstance( unsigned int uiSectID, unsigned char * Instance, unsigned char Size );
extern bool ReadSingleSectorMultiInstance( unsigned int uiSectID, unsigned char * Instance, unsigned char Size );

extern bool FLASH_Init( void );
extern bool FLASH_Format( unsigned char Type );

// App Functions Definition
extern bool FLASH_AppendRecord( void );
extern bool FLASH_AppendRecord_Local( void );

extern bool FLASH_GPRSReadRecord( void );
extern bool FLASH_GPRSIsRecordEmpty( void );
extern bool FLASH_GPRSDeleteRecord( void );
extern bool FLASH_GPRSDeleteLocalRecord( void );
extern bool FLASH_ReadLocalRecord( void );

/* Config Functions Define ---------------------------------------------------------------------*/

// 常用信息
#define FLASH_WriteCommonConfig()		WriteSinglePageSingleInstance( FLASH_SECT_COMMON_CONFIG, (uchar *)g_pCommonConfig, sizeof(COMMON_SETTING))
#define FLASH_ReadCommonConfig()		ReadSinglePageSingleInstance( FLASH_SECT_COMMON_CONFIG, (uchar *)g_pCommonConfig, sizeof(COMMON_SETTING))

// 特殊配置信息
#define FLASH_WriteSpecificConfig()		WriteSinglePageSingleInstance( FLASH_SECT_SPECIFIC_SETTING, (uchar *)g_pSpecificConfig, sizeof(SPECIFIC_SETTING))
#define FLASH_ReadSpecificConfig()		ReadSinglePageSingleInstance( FLASH_SECT_SPECIFIC_SETTING, (uchar *)g_pSpecificConfig, sizeof(SPECIFIC_SETTING))

// 参数配置信息
#define FLASH_WriteParameterConfig()	WriteSinglePageSingleInstance( FLASH_SECT_PARAMETER_SETTING, (uchar *)g_pParameterConfig, sizeof(PARAMETER_SETTING))
#define FLASH_ReadParameterConfig()		ReadSinglePageSingleInstance( FLASH_SECT_PARAMETER_SETTING, (uchar *)g_pParameterConfig, sizeof(PARAMETER_SETTING))

// EDGE网络IP地址配置信息
#define FLASH_WriteEDGEIPAddrConfig()	WriteSinglePageSingleInstance( FLASH_SECT_COM_EDGE_IPADDR, (uchar *)g_pCOM_EDGE_IPAddrConfig, sizeof(COM_SET_IPADDR_CONFIG))
#define FLASH_ReadEDGEIPAddrConfig()	ReadSinglePageSingleInstance( FLASH_SECT_COM_EDGE_IPADDR, (uchar *)g_pCOM_EDGE_IPAddrConfig, sizeof(COM_SET_IPADDR_CONFIG))

// CDMA网络IP地址配置信息
#define FLASH_WriteCDMAIPAddrConfig()	WriteSinglePageSingleInstance( FLASH_SECT_COM_CDMA_IPADDR, (uchar *)g_pCOM_CDMA_IPAddrConfig, sizeof(COM_SET_IPADDR_CONFIG))
#define FLASH_ReadCDMAIPAddrConfig()	ReadSinglePageSingleInstance( FLASH_SECT_COM_CDMA_IPADDR, (uchar *)g_pCOM_CDMA_IPAddrConfig, sizeof(COM_SET_IPADDR_CONFIG))

// 油量纪录
//#define	FLASH_WriteGasValue()		WriteSingleSectorMultiInstance( FLASH_SECT_GASVALUE, (uchar *)&(g_pStatusInfo->GasValue), sizeof(g_pStatusInfo->GasValue), (uchar)((uint)FLASH_PAGE_GASVALUE_BACKUP-FLASH_PAGE_GASVALUE) )
//#define	FLASH_ReadGasValue()		ReadSingleSectorMultiInstance ( FLASH_SECT_GASVALUE, (uchar *)&(g_pStatusInfo->GasValue), sizeof(g_pStatusInfo->GasValue), (uchar)((uint)FLASH_PAGE_GASVALUE_BACKUP-FLASH_PAGE_GASVALUE) )

// 最后站点记录
#define	FLASH_WriteLastStation()		WriteSingleSectorMultiInstance( FLASH_SECT_LASTSTATION, (uchar *)g_pLastStationInfo, sizeof(LASTSTATION_INFO) )
#define	FLASH_ReadLastStation()			ReadSingleSectorMultiInstance ( FLASH_SECT_LASTSTATION, (uchar *)g_pLastStationInfo, sizeof(LASTSTATION_INFO) )

// 里程记录表
#define	FLASH_WriteMileMeter()			WriteSingleSectorMultiInstance( FLASH_SECT_MILEMETER, (uchar *)&g_uiMileMeter, sizeof(g_uiMileMeter) )
#define	FLASH_ReadMileMeter()			ReadSingleSectorMultiInstance ( FLASH_SECT_MILEMETER, (uchar *)&g_uiMileMeter, sizeof(g_uiMileMeter) )

// 线路信息
#define FLASH_WriteBusInfo()			WriteSinglePageSingleInstance( FLASH_SECT_BUSINFO, (uchar *)g_pBusInfo, sizeof(BUS_INFO))
#define FLASH_ReadBusInfo()				ReadSinglePageSingleInstance( FLASH_SECT_BUSINFO, (uchar *)g_pBusInfo, sizeof(BUS_INFO))

// 行车主体配置信息
#define FLASH_WriteThemeConfig()		WriteSinglePageSingleInstance( FLASH_SECT_THEME, (uchar *)g_pThemeInfo, sizeof(THEME_INFO))
#define FLASH_ReadThemeConfig()			ReadSinglePageSingleInstance( FLASH_SECT_THEME, (uchar *)g_pThemeInfo, sizeof(THEME_INFO))

// Flash Device Info
#define FlashWriteDeviceInfo(a)			WritePerSectorMultiInstance( (uint)(FLASH_SECT_DEVICEINFO), (unsigned char *)(a), sizeof(FLASH_INFO) )	
#define FlashReadDeviceInfo(a)			ReadPerSectorMultiInstance( (uint)(FLASH_SECT_DEVICEINFO), (unsigned char *)(a), sizeof(FLASH_INFO) )

// WSB
#define FlashWriteWsbConfig(a)			WritePerSectorMultiInstance( (uint)(FLASH_SECT_WSB_MODE), (unsigned char *)(a), (uchar)sizeof(WSB_CONFIG_INFO) )	
#define FlashReadWsbConfig(a)			ReadPerSectorMultiInstance( (uint)(FLASH_SECT_WSB_MODE), (unsigned char *)(a), (uchar)sizeof( WSB_CONFIG_INFO) )

// Termainal  No
#define FlashWriteTerminalNo(a)			WriteSinglePageSingleInstance( (uint)(FLASH_SECT_TERMINALNO), (unsigned char *)(a), 4 )	
#define FlashReadTerminalNo(a)			ReadSinglePageSingleInstance( (uint)(FLASH_SECT_TERMINALNO), (unsigned char *)(a), 4 )

//---------------------------------------------------------------
extern bool FLASH_WriteCommonConfigInfo( void );
extern void CheckIapSign( void );
#endif  /* End of the file */



