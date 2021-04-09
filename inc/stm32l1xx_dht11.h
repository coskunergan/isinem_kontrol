/**
  ******************************************************************************
  * @file    stm32l1xx_dht11.h
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    28-Agustos-2014
  * @brief   DHT11 Isi Nem Sensöru kütüpanesi
  *
	*  @verbatim
********************************************************************************/

/* simple delay based dht11 library */
/* author: Adam Orcholski, www.tath.eu, tath@o2.pl */

#ifndef _dht_def_
#define _dht_def_

#include "stdint.h"
#include "stm32l1xx.h" /* hardware dependent */

#define enableInterrupts()   __set_PRIMASK(0);
#define disableInterrupts()  __set_PRIMASK(1);
/**************** Timing *****************/
#define DHT11_Delay_1uS  		__NOP();
#define DHT11_Delay_10uS 		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
#define DHT11_Delay_20uS	 	DHT11_Delay_10uS; DHT11_Delay_10uS; DHT11_Delay_10uS;
#define DHT11_Delay_40uS	 	DHT11_Delay_20uS; DHT11_Delay_20uS;	DHT11_Delay_10uS;
#define DHT11_Delay_80uS	 	DHT11_Delay_40uS; DHT11_Delay_40uS;
#define DHT11_Delay_100uS   DHT11_Delay_80uS;DHT11_Delay_20uS;
/*************** Constants ***************/

/* GPIO port name and number for 1-wire data input/output */
#define Dht_Pin_Port      GPIOC
#define Dht_Pin				    GPIO_Pin_8

#define Dht_Pin_Read 	((Dht_Pin_Port -> IDR & Dht_Pin)!=0x0)

#define Dht_Pin_High  Dht_Pin_Port->BSRRL =Dht_Pin;
#define Dht_Pin_Low   Dht_Pin_Port->BSRRH =Dht_Pin;

/* optional timeouts for while() loops (in case of hardware failure) */
#define TIMEOUT_VALUE        50000

/* return codes : */
#define DHT11_OK 0
#define DHT11_TIMEOUT 1
#define DHT11_WRONG_CHCKSUM 2

/* prototypes: */
uint8_t DHT11_Oku(float *isi, float *nem);
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
