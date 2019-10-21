/**
  ******************************************************************************
  * @file    stm32l1xx_int_eeprom.c
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    15 Subat 2013
  * @brief   eeprom haberlesmesi kontrol edildigi dosya
  ******************************************************************************
  * @copy
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32l1xx_int_eeprom.h"

__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;

/***************************************************************************************/
void Int_Eeprom_ReadStr(uint32_t Addr, uint8_t *buff_data, uint8_t len)  // eeprom adresindeki diziyi okur
{
    uint32_t  Addr_End = Addr + len + DATA_EEPROM_START_ADDR;

    Addr += DATA_EEPROM_START_ADDR;
    disableInterrupts();
    DATA_EEPROM_Unlock();

    while(Addr < Addr_End)
    {
        /* eeprom okunuyor. */
        *buff_data = *(__IO uint8_t *)Addr;
        buff_data++;
        Addr = Addr + 1;
    }

    DATA_EEPROM_Lock();
    enableInterrupts();
}
/**************************************************************************************/
void Int_Eeprom_WriteStr(uint32_t Addr, uint8_t *buff_data, uint8_t len) // eeprom adresindeki dizinin üzerine yazar
{
    uint32_t  Addr_End = Addr + len + DATA_EEPROM_START_ADDR;

    Addr += DATA_EEPROM_START_ADDR;
    disableInterrupts();
    DATA_EEPROM_Unlock();

    while(Addr < Addr_End)
    {
        /* Adrese veri yaziliyor */
        FLASHStatus = DATA_EEPROM_ProgramByte(Addr, *buff_data);

        if(FLASHStatus == FLASH_COMPLETE)
        {
            Addr = Addr + 1;
            buff_data++;
        }
        else
        {
            FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                            | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
        }
    }

    DATA_EEPROM_Lock();
    enableInterrupts();
}
/**************************************************************************************/

