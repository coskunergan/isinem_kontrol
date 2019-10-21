/**
  ******************************************************************************
  * @file    stm32l1xx_sht11.c
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    25-Agustos-2014
  * @brief   SHT11 Isi Nem Sensöru kütüpanesi
  *
  *  @verbatim
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_sht11.h"

/** @addtogroup sensör kütüpanesi
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***************** PORT *******************/
#define SHT_SDA_GPIO  STH11_Sda_Port
#define SHT_SDA_PIN   STH11_Sda_Pin
#define SHT_SCL_GPIO  STH11_Scl_Port
#define SHT_SCL_PIN	  STH11_Scl_Pin
/**************** Control *****************/
#define Sda 	((SHT_SDA_GPIO->IDR & SHT_SDA_PIN)!=0x0)
#define SCL_LOW    SHT_SCL_GPIO->BSRRH = SHT_SCL_PIN
#define SCL_HIGH   SHT_SCL_GPIO->BSRRL = SHT_SCL_PIN
#define SDA_LOW   SHT_SDA_GPIO->BSRRH = SHT_SDA_PIN
#define SDA_HIGH  SHT_SDA_GPIO->BSRRL = SHT_SDA_PIN
/**************** define *****************/
#define Tamam	0
#define	Hata	1
#define Isi_Adres 0x03
#define Nem_Adres 0x05
/**************** Timing *****************/
#define SHT11_Delay_1  __NOP();
#define SHT11_Delay_2  __NOP();__NOP();
/*************** Constants ***************/
const float C1 = -2.0468; // 12bit temp , 8 bit RH sabitleri
const float C2 = +0.0367;
const float C3 = -0.0000015955;
const float T1 = +0.01;
const float T2 = +0.00128;
/******************************************************************************/
/******************************************************************************/
/***                      SHT11 Sensör Haberlesme Programi                  ***/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
void SHT11_NAck(void)
{
    SDA_LOW;
    SHT11_Delay_2;
    SCL_HIGH;
    SHT11_Delay_2;
    SCL_LOW;
    SHT11_Delay_2;
    SDA_HIGH;
}
/******************************************************************************/
bool SHT11_Ack(void)
{
    SDA_HIGH;
    SCL_LOW;
    SHT11_Delay_2;
    SCL_HIGH;
    SHT11_Delay_2;
    return(Sda);
}
/******************************************************************************/
void SHT11_Gonder8Bit(uint8_t x)
{
    uint8_t i = 0x80;
    do
    {
        SCL_LOW;
        if(x & i)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        SHT11_Delay_1;
        SCL_HIGH;
        i >>= 1;
        SHT11_Delay_1;
    }
    while(i != 0);
}
/****************************************************************************/
void SHT11_Reset(void)
{
    SCL_HIGH;
    SDA_HIGH;
    SHT11_Gonder8Bit(0xFF); // 8 reset biti
    SCL_LOW;
    SHT11_Delay_2;
    SCL_HIGH;	    // 9. reset biti
    SHT11_Delay_1;
}
/****************************************************************************/
void SHT11_Start(void)
{
    SCL_LOW;
    SHT11_Delay_2;
    SCL_HIGH;
    SHT11_Delay_2;
    SDA_LOW;
    SHT11_Delay_2;
    SCL_LOW;
    SHT11_Delay_2;  // start bits
    SHT11_Delay_2;
    SCL_HIGH;
    SHT11_Delay_2;
    SDA_HIGH;
    SHT11_Delay_2;
    SCL_LOW;
    SHT11_Delay_2;
    SDA_LOW;
    SHT11_Delay_2;
}
/******************************************************************************/
uint8_t SHT11_Oku8Bit(void)
{
    uint8_t x = 0, i = 0x80;

    do
    {
        SCL_HIGH;
        SHT11_Delay_1;
        if(Sda)
        {
            x = (x) | i;
        }
        SHT11_Delay_1;
        SCL_LOW;
        i >>= 1;
        SHT11_Delay_1;
    }
    while(i != 0);

    return x;
}
/******************************************************************************/
uint8_t SHT11_Paket_Al(uint8_t secim, float *ham)
{
    uint16_t temp;
    uint32_t Timeout = 20000; // 1 mhz için 480ms

    SHT11_Reset();
    SHT11_Start();
    SHT11_Gonder8Bit(secim);
    if(SHT11_Ack())
    {
        return 1;
    }
    SCL_LOW;
    SHT11_Delay_2;
    SHT11_Delay_2;

    do
    {
        Timeout--;
        if(!Timeout)
        {
            return 1;
        }
    }
    while(Sda);

    temp = SHT11_Oku8Bit();
    SHT11_NAck();
    temp <<= 8;
    temp |= SHT11_Oku8Bit();
    SHT11_Ack();
    *ham = temp;

    return 0;
}
/******************************************************************************/
void SHT11_Hesapla(float *isi_ham, float *nem_ham)
{
    float nem = *nem_ham;
    float isi = *isi_ham;
    float nem_lin;
    float nem_gercek;
    float isi_gercek;

    isi_gercek = isi * 0.01 - 39.7;
    nem_lin = C3 * nem * nem + C2 * nem + C1;
    nem_gercek = (isi_gercek - 25) * (T1 + T2 * nem) + nem_lin;

    if(nem_gercek > 99)
    {
        nem_gercek = 99;
    }
    if(nem_gercek < 0.1)
    {
        nem_gercek = 0.1;
    }

    *nem_ham = nem_gercek;
    *isi_ham = isi_gercek;

}
/*****************************************************************************/
uint8_t SHT11_Oku(float *isi, float *nem)
{
    float nem_ham, isi_ham;
    uint8_t err = 0;

    err |= SHT11_Paket_Al(Isi_Adres, &isi_ham);
    err |= SHT11_Paket_Al(Nem_Adres, &nem_ham);

    if(err)
    {
        *isi = 0;
        *nem = 0;
        return (Hata);
    }
    else
    {
        SHT11_Hesapla(&isi_ham, &nem_ham);
        *isi = isi_ham;
        *nem = nem_ham;
        return (Tamam);
    }
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/

