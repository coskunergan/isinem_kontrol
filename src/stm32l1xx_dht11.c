/**
  ******************************************************************************
  * @file    stm32l1xx_dht11.c
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    28-Agustos-2014
  * @brief   DHT11 Isi Nem Sensöru kütüpanesi
  *
  *  @verbatim
*******************************************************************************/

#include "stm32l1xx_dht11.h"

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
void DHT_Delay100Us(uint32_t time)
{
    while(time--)
    {
        DHT11_Delay_100uS;
    }
}
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
uint8_t DHT11_Oku(float *isi, float *nem)
{
    uint8_t i, j = 0, cnt = 7, p[5];
    uint16_t timeout = TIMEOUT_VALUE;
    uint32_t per;

    for(i = 0; i < 5; i++)
    {
        p[i] = 0;
    }
    Dht_Pin_Low;

    DHT_Delay100Us(180);

    disableInterrupts();

    Dht_Pin_High;

    DHT11_Delay_40uS;

    while(Dht_Pin_Read == 0) /* 80us on '0' */
    {
        if(timeout-- == 0x0)
        {
            enableInterrupts();
            return DHT11_TIMEOUT;
        }
    }

    while(Dht_Pin_Read == 1) /* 80us on '1' */
    {
        if(timeout-- == 0x0)
        {
            enableInterrupts();
            return DHT11_TIMEOUT;
        }
    }

    for(i = 0; i < 40; i++)
    {
        while(Dht_Pin_Read == 0)
        {
            if(timeout-- == 0)
            {
                enableInterrupts();
                return DHT11_TIMEOUT;
            }
        } /* 50 us s1f1r gelir 11 birim */

        per = SysTick->VAL;

        while(Dht_Pin_Read == 1)
        {
            if(timeout-- == 0)
            {
                enableInterrupts();
                return DHT11_TIMEOUT;
            }
        } /* 50 us s1f1r gelir 11 birim */

        if((per - SysTick->VAL) >= 5)
        {
            p[j] |= (1 << cnt);
        }
        if(cnt == 0)    // next byte?
        {
            cnt = 7;    // restart at MSB
            j++;      // next byte!
        }
        else
        {
            cnt--;
        }
    }

    enableInterrupts();
    /* checksum check */
    if((p[0] + p[1] + p[2] + p[3]) != p[4])
    {
        return DHT11_WRONG_CHCKSUM;
    }

    *isi = p[2];
    *isi += (float)(p[3] * 0.1);
    *nem = p[0];
    *nem += (float)(p[1] * 0.1);

    return DHT11_OK;

}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/

