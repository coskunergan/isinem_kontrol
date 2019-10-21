/**
  ******************************************************************************
  * @file    stm32l1xx_sht11.h
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    25-Agustos-2014
  * @brief   SHT11 Isi Nem Sensöru kütüpanesi 
  *
	*  @verbatim
********************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_SHT11_H
#define __STM32L1xx_SHT11_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "board_define.h"
	
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define enableInterrupts()   __set_PRIMASK(0);
#define disableInterrupts()  __set_PRIMASK(1);

uint8_t SHT11_Oku(float *isi, float *nem); 
uint8_t SHT11_Yaz_Config(uint8_t status);


#ifdef __cplusplus
}
#endif

#endif /*__STM32L1xx_sht11_H */

/**
  * @}
  */ 

/**
  * @}
  */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
