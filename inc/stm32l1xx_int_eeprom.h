/**
  ******************************************************************************
  * @file    Header File
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    15-Subat-2013
  * @brief   Bu Bir Hedeader dosyadir
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32l1xx.h"

#define enableInterrupts()   __set_PRIMASK(0);
#define disableInterrupts()  __set_PRIMASK(1);

#define DATA_EEPROM_START_ADDR      0x08080000
#define DATA_EEPROM_END_ADDR        0x080803FF

void Int_Eeprom_ReadStr(uint32_t Addr, uint8_t *buff_data, uint8_t len);
void Int_Eeprom_WriteStr(uint32_t Addr, uint8_t *buff_data, uint8_t len);

#endif  /* __H */

/*******************(C)COPYRIGHT 2012 STMicroelectronics *****END OF FILE******/
