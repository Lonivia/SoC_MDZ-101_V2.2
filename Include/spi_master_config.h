/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 *
 * @defgroup spi_master_example_main spi_master_config
 * @{
 * @ingroup spi_master_example
 *
 * @brief Defintions needed for the SPI master driver.
 */
 
#ifndef SPI_MASTER_CONFIG_H__
#define SPI_MASTER_CONFIG_H__

#define SPI_OPERATING_FREQUENCY (0x02000000uL << (uint32_t)Freq_1Mbps)  /**< Slave clock frequency. */

#if( HW_VER == HW_VER_12 )

	// SPI0. 
	#define SPI_PSELSCK0            8u                                     
	#define SPI_PSELMOSI0           9u                                    
	#define SPI_PSELMISO0           10u                                     
	#define SPI_PSELSS0             12u      					                     	

	// SPI1.
	#define SPI_PSELSCK1            26u                                     
	#define SPI_PSELMOSI1           28u                                     
	#define SPI_PSELMISO1           29u                                    
	#define SPI_PSELSS1             27u                                     
                                   

#elif(HW_VER == HW_VER_13 )
	
	// SPI0. 
	#define SPI_PSELSCK0            11u                                     /**< SPI clock GPIO pin number. */
	#define SPI_PSELMOSI0           10u                                     /**< SPI Master Out Slave In GPIO pin number. */
	#define SPI_PSELMISO0           9u                                     /**< SPI Master In Slave Out GPIO pin number. */
	#define SPI_PSELSS0             7u      					                     	/**< SPI Slave Select GPIO pin number. */

	// SPI1.
	#define SPI_PSELSCK1            29u                                     /**< SPI clock GPIO pin number. */
	#define SPI_PSELMOSI1           28u                                     /**< SPI Master Out Slave In GPIO pin number. */
	#define SPI_PSELMISO1           4u                                     /**< SPI Master In Slave Out GPIO pin number. */
	#define SPI_PSELSS1             3u 

#endif


#define TIMEOUT_COUNTER         0x3000uL                                /**< Timeout for SPI transaction in units of loop iterations. */

#endif // SPI_MASTER_CONFIG_H__

/** @} */
