/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Template/stm32l1xx_it.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    May-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_rtc.h"
#include "board_define.h"
#include "stm32l1xx_int_eeprom.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern  xSemaphoreHandle xEncoderSemaphore;
extern  xSemaphoreHandle xButonSemaphore;
extern  xSemaphoreHandle xRTCSemaphore;
extern uint8_t Dimmer;
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
void ButonB_IRQHandler(void)
{
    EXTI_ClearITPendingBit(ButonB_ExtiLine);

    xSemaphoreGiveFromISR(xButonSemaphore, NULL);
}
/******************************************************************************/
void EncoderInt_IRQHandler(void) // encoder pot interrupt
{
    EXTI_ClearITPendingBit(EncoderInt_ExtiLine);

    xSemaphoreGiveFromISR(xEncoderSemaphore, NULL);
}
/******************************************************************************/
void Dimmer_Timer_IRQHandler(void)  // dimmer timer overflow int
{
    TIM_ClearITPendingBit(Dimmer_Timer, TIM_IT_Update);
    TIM_Cmd(Dimmer_Timer, DISABLE);
    Gpio_High(Moc2030_Pulse_Port, Moc2030_Pulse_Pin);
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP(); // Pulse MOC2030 20us
    Gpio_Low(Moc2030_Pulse_Port, Moc2030_Pulse_Pin);
}
/******************************************************************************/
void Zero_Cross_IRQHandler(void) // zero cross detect interrupt
{
    static uint32_t Period = 0;
    static uint32_t Period_Avr = 0;
    static uint8_t Period_Index = 0;
    uint32_t temp;

    EXTI_ClearITPendingBit(Zero_Detect_ExtiLine);
//    if(Period_Value >= 12000)
//    {
//				Period_Value=0;
//        return;
//    }		
    //--------------------
    if(Dimmer == 99)// full ise tamamen ac
    {
        Gpio_High(Moc2030_Pulse_Port, Moc2030_Pulse_Pin);
				Period_Avr += Period_Value;
				Period_Value = 0;
				Period_Index++;

				if(Period_Index >= 16)
				{
						Period_Index = 0;
						Period = Period_Avr >> 4;
						Period_Avr = 0;
				}			
    }
    else if(Dimmer == 0 || Period == 0)
    {
        Gpio_Low(Moc2030_Pulse_Port, Moc2030_Pulse_Pin);
				Period_Avr += Period_Value;
				Period_Value = 0;
				Period_Index++;

				if(Period_Index >= 16)
				{
						Period_Index = 0;
						Period = Period_Avr >> 4;
						Period_Avr = 0;
				}			
    }
    else
    {
        Gpio_Low(Moc2030_Pulse_Port, Moc2030_Pulse_Pin);
        temp = Period / 2;
        Dimmer_Value = (65535 - temp) + ((temp / 110) * (Dimmer));
        TIM_Cmd(Dimmer_Timer, ENABLE);
    }
    //--------------------
}
/******************************************************************************/
void HardFault_Handler_C(unsigned long *hardfault_args)
{
    Int_Eeprom_WriteStr(0x300, (unsigned char *)&hardfault_args[5], 4); // LR_Hata
    Int_Eeprom_WriteStr(0x304, (unsigned char *)&hardfault_args[6], 4); // PC_Hata

    while(1)
    {
    }
}
#ifdef __ICCARM__
void   HardFault_Handler(void)
{
    asm("TST    LR, #4");//
    asm("ITE EQ");
    asm("MRSEQ R0, MSP");
    asm("MRSNE R0, PSP");
    asm("B HardFault_Handler_C");
}
#else // MDKARM
__asm void HardFault_Handler(void)
{
    TST LR, #4
    ITE EQ
    MRSEQ R0, MSP
    MRSNE R0, PSP
    B __cpp(HardFault_Handler_C)
}
#endif
/******************************************************************************/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while(1);
}
/******************************************************************************/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while(1);
}
/******************************************************************************/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while(1);
}
/******************************************************************************/
void DebugMon_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while(1);
}
/******************************************************************************/
void RTC_WKUP_IRQHandler(void)
{
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20);
    xSemaphoreGiveFromISR(xRTCSemaphore, NULL);
}
/******************************************************************************/
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
/******************************************************************************/
