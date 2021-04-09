/**
  ******************************************************************************
  * @file    init.c
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    15 Subat 2013
  * @brief   init
  ******************************************************************************
  * @copy
  */

/* Includes ------------------------------------------------------------------*/
#include "hw_init.h"

#define DEBUG_SWD_ACTIVE     1    //  debug aktif 

/****************************************************************************************/
void HW_Setup(void)
{
    /* GPIO, EXTI and NVIC Init structure declaration */
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    bool LSI_Enable = false;
    uint32_t Timeout_LSE = 20000;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

///////////////////////// RTC INIT //////////////////////////
    PWR_RTCAccessCmd(ENABLE);

    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);

    RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
        Timeout_LSE--;
        if(Timeout_LSE == 0)
        {
            break;
        }
    }
    if(Timeout_LSE == 0)
    {
        RCC_LSEConfig(RCC_LSE_OFF);
        /*!< LSI Enable */
        RCC_LSICmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {}
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        LSI_Enable = true;
    }
    else
    {
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    }

    RCC_RTCCLKCmd(ENABLE);

    RCC_ClockSecuritySystemCmd(ENABLE);
    RCC_LSEClockSecuritySystemCmd(ENABLE);

    RTC_WaitForSynchro();

    /* RTC Wakeup Interrupt Generation: Clock Source: RTCDiv_16  */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);

    if(LSI_Enable == true)
    {
        /***LSI kristal degerleri***/
// 0xE9   = 100ms
// 0x48F  = 500ms
// 0x91F  = 1sn
// 0x1D20 = 2sn
// 0x3A40 = 4sn
// 0xFFFF = 28.2sn
        RTC_SetWakeUpCounter(0x91F);
    }
    else
    {
        /***LSE kristal degerleri***/
// 0xCC   = 100ms
// 0x400  = 500ms
// 0x800  = 1sn
// 0x2000 = 4sn
// 0xF000 = 30sn
// 0xFFFF = 32sn
        RTC_SetWakeUpCounter(0x800);
    }
    /* Enable the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    RTC_WakeUpCmd(ENABLE);
/////////////////////// GPIO Pin Ayarlari //////////////////////

#if DEBUG_SWD_ACTIVE  == 1
    DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);
#else
    DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, DISABLE);
#endif

    PWR_UltraLowPowerCmd(ENABLE);

    /* Enable the GPIOs clocks. */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD |
                          RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOH | RCC_AHBPeriph_GPIOG, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;

    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_Init(GPIOH, &GPIO_InitStructure);
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);

#if DEBUG_SWD_ACTIVE  == 1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All & (~GPIO_Pin_13) & (~GPIO_Pin_14);
#endif
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Disable GPIOs clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOH | RCC_AHBPeriph_GPIOG, DISABLE);

    ////////////////////// Out Pins //////////////////////////////
    GPIO_InitStructure.GPIO_Pin = Moc2030_Pulse_Pin;    // MOC2030 PULSE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(Moc2030_Pulse_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;    // BUZZER
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(Buzzer_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FM_Enb_Pin;    // FM ENABLE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FM_Enb_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STH11_Vcc_Pin;    // FM ENABLE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(STH11_Vcc_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Lcd_Si_Pin | Lcd_Scl_Pin | Lcd_A0_Pin | Lcd_Enb_Pin | Lcd_BL_Pin; // GSM reset
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(Gsm_Rst_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STH11_Sda_Pin;    // SHT11 I2C-SDA pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(STH11_Sda_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STH11_Scl_Pin;    // SHT11 I2C-SCL pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(STH11_Scl_Port, &GPIO_InitStructure);
////////////////////// Input Pins ////////////////////////////
    GPIO_InitStructure.GPIO_Pin = Zero_Detect_Pin;  // Zero Cross Detect
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(Zero_Detect_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Encoder_1A_Pin; // ENCODER - A
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(Encoder_1A_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Encoder_1B_Pin;  // ENCODER - B
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(Encoder_1B_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ButonB_Pin;  // Buton - B
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(ButonB_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = EncoderInt_Pin;  // encoder okuma int
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(EncoderInt_Port, &GPIO_InitStructure);
//////////////////// High Level Pins /////////////////////////
    Gpio_High(Buzzer_Port, Buzzer_Pin);
    Gpio_High(FM_Enb_Port, FM_Enb_Pin);
    Gpio_High(Lcd_BL_Port, Lcd_BL_Pin);
    Gpio_High(STH11_Vcc_Port, STH11_Vcc_Pin);
    Gpio_High(STH11_Sda_Port, STH11_Sda_Pin);
    Gpio_High(STH11_Scl_Port, STH11_Scl_Pin);
////////////////////////// Buton interrupt ////////////////////
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, ButonB_ExtiPin);
    SYSCFG_EXTILineConfig(EncoderInt_ExtiPort, EncoderInt_ExtiPin);
    SYSCFG_EXTILineConfig(Zero_Detect_ExtiPort, Zero_Detect_ExtiPin);

    EXTI_InitStructure.EXTI_Line = ButonB_ExtiLine | Zero_Detect_ExtiLine | EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line20;//RTC_WKup
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line =  	EncoderInt_ExtiLine ;
    EXTI_InitStructure.EXTI_Mode = 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_ClearITPendingBit(ButonB_ExtiLine);
    EXTI_ClearITPendingBit(EncoderInt_ExtiLine);
    EXTI_ClearITPendingBit(Zero_Detect_ExtiLine);
/////////////////////////// BUTON EVENT INTERRUPT ////////////////////
    NVIC_InitStructure.NVIC_IRQChannel = ButonB_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
//////////////////// ENCODER POT EVENT INTERRUPT /////////////////////
    NVIC_InitStructure.NVIC_IRQChannel = EncoderInt_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
///////////////////  ZERO CROSS DETECT INTERRUPT //////////////(//////
    NVIC_InitStructure.NVIC_IRQChannel = Zero_Detect_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
///////////////////// INIT Wkup INTERRUPT  ///////////////////////////
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
//////////////////////// ENCODER TIMER INIT //////////////////////////
    RCC_APB1PeriphClockCmd(Encoder_Timer_Clk, ENABLE);

    GPIO_PinAFConfig(Encoder_1A_Port, Encoder_1A_Source, Encoder_GPIO_AF);
    GPIO_PinAFConfig(Encoder_1B_Port, Encoder_1B_Source, Encoder_GPIO_AF);

    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_Period = 600;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(Encoder_Timer, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(Encoder_Timer, ENABLE);

    TIM_EncoderInterfaceConfig(Encoder_Timer, TIM_EncoderMode_TI2,  TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
//////////////////////// DIMMER TIMER INIT //////////////////////////
    RCC_APB1PeriphClockCmd(Dimmer_Timer_Clk, ENABLE);
    RCC_APB1PeriphClockCmd(Period_Timer_Clk, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 0; // frekans periyot aral1klar1 zamanlamay1 uzat1r
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_Period = 65535;  // 0 ile 100 aras1nda deger yüklerim  ve 100 den sonras1 zero geçisinden sonraki pals giri_inin beklemesidir.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(Dimmer_Timer, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(Dimmer_Timer, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(Period_Timer, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel = Dimmer_Timer_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(Dimmer_Timer, TIM_IT_Update, ENABLE);
    TIM_Cmd(Dimmer_Timer, DISABLE);
    TIM_Cmd(Period_Timer, ENABLE);
}
/*************************************************************************************/

