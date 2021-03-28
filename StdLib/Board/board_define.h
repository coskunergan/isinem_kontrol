 /**
  ******************************************************************************
  * @file    board_define.h
  * @author  Coskun ERGAN
  * @version V1.0.3
  * @date    Agust-2014
  * @brief   Input/Output defines
  ******************************************************************************
  * @copy
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __BOARD_DEFINE_H
#define __BOARD_DEFINE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"  
#include <stdbool.h>

/* MACROs for SET, RESET or TOGGLE Output port */

#define Gpio_Get(a,b) 				((a->IDR & b)!=0x0)
#define Gpio_High(a,b) 				a->BSRRL = b
#define Gpio_Low(a,b)					a->BSRRH = b
#define Gpio_Toggle(a,b) 			a->ODR ^= b 

#define Moc2030_Pulse_Port	  GPIOC
#define Moc2030_Pulse_Pin     GPIO_Pin_0
#define Moc2030_Pulse_Clk     RCC_AHBPeriph_GPIOC

#define Zero_Detect_Port	  	GPIOC
#define Zero_Detect_Pin     	GPIO_Pin_7
#define Zero_Detect_Clk     	RCC_AHBPeriph_GPIOC
#define Zero_Detect_Source  	GPIO_PinSource7
#define Zero_Detect_ExtiPin  	EXTI_PinSource7
#define Zero_Detect_ExtiPort 	EXTI_PortSourceGPIOC
#define Zero_Detect_ExtiLine 	EXTI_Line7
#define Zero_Detect_IRQn		  EXTI9_5_IRQn
#define Zero_Cross_IRQHandler EXTI9_5_IRQHandler
 
#define STH11_Scl_Port 				GPIOC
#define STH11_Scl_Pin 				GPIO_Pin_9
#define STH11_Scl_Clk       	RCC_AHBPeriph_GPIOC

#define STH11_Sda_Port 				GPIOC
#define STH11_Sda_Pin 				GPIO_Pin_8
#define STH11_Sda_Clk       	RCC_AHBPeriph_GPIOC

#define STH11_Vcc_Port 				GPIOA
#define STH11_Vcc_Pin 				GPIO_Pin_8
#define STH11_Vcc_Clk       	RCC_AHBPeriph_GPIOA

#define Buzzer_Port 					GPIOA
#define Buzzer_Pin 						GPIO_Pin_11
#define Buzzer_Clk       			RCC_AHBPeriph_GPIOA
/*  FM Enb Pin tanimlamasi */
#define FM_Enb_Pin						GPIO_Pin_12
#define FM_Enb_Port					GPIOA
#define FM_Enb_Clk						RCC_AHBPeriph_GPIOA
/*  GSM Reset Pin tanimlamasi */
#define Gsm_Rst_Pin   			GPIO_Pin_12
#define Gsm_Rst_Port				GPIOC
#define Gsm_Rst_Clk				RCC_AHBPeriph_GPIOC

#define ButonB_Pin   					GPIO_Pin_0
#define ButonB_Port						GPIOA
#define ButonB_Clk						RCC_AHBPeriph_GPIOA
#define ButonB_Source  				GPIO_PinSource0
#define ButonB_ExtiPin  			EXTI_PinSource0
#define ButonB_ExtiPort 			EXTI_PortSourceGPIOA
#define ButonB_ExtiLine 			EXTI_Line0
#define ButonB_IRQn		    		EXTI0_IRQn
#define ButonB_IRQHandler 		EXTI0_IRQHandler
 
#define EncoderInt_Port	  		GPIOA
#define EncoderInt_Pin     		GPIO_Pin_2
#define EncoderInt_Clk     		RCC_AHBPeriph_GPIOA
#define EncoderInt_Source  		GPIO_PinSource2
#define EncoderInt_ExtiPin  	EXTI_PinSource2
#define EncoderInt_ExtiPort 	EXTI_PortSourceGPIOA
#define EncoderInt_ExtiLine 	EXTI_Line2
#define EncoderInt_IRQn		    EXTI2_IRQn
#define EncoderInt_IRQHandler EXTI2_IRQHandler

#define Encoder_1A_Port				GPIOB
#define Encoder_1A_Pin 				GPIO_Pin_4
#define Encoder_1A_Source 		GPIO_PinSource4
#define Encoder_1A_Clk       	RCC_AHBPeriph_GPIOB

#define Encoder_1B_Port 			GPIOB
#define Encoder_1B_Pin 				GPIO_Pin_5
#define Encoder_1B_Source 		GPIO_PinSource5
#define Encoder_1B_Clk       	RCC_AHBPeriph_GPIOB

#define Encoder_Timer       	TIM3
#define Encoder_Timer_Clk     RCC_APB1Periph_TIM3
#define Encoder_GPIO_AF       GPIO_AF_TIM3
#define Encoder_Timer_Value  	TIM3->CNT

#define Dimmer_Timer       		  TIM5
#define Dimmer_Timer_Clk     	  RCC_APB1Periph_TIM5
#define Dimmer_Value	       	  TIM5->CNT
#define Dimmer_PreValue     	  TIM5->PSC

#define Dimmer_Timer_IRQn       TIM5_IRQn
#define Dimmer_Timer_IRQHandler TIM5_IRQHandler

#define Period_Timer       		  TIM6
#define Period_Timer_Clk     	  RCC_APB1Periph_TIM6
#define Period_Value	       	  TIM6->CNT
////////////////////////////////////////////////////
//																								//
//					 LCD ST7522 Tanimlamalari							//
//																								//
////////////////////////////////////////////////////
#define  Lcd_Si_Pin						GPIO_Pin_14
#define  Lcd_Si_Port					GPIOB
#define  Lcd_Si_Clk						RCC_AHBPeriph_GPIOB  
  
#define  Lcd_Scl_Pin					GPIO_Pin_2
#define  Lcd_Scl_Port					GPIOB
#define  Lcd_Scl_Clk					RCC_AHBPeriph_GPIOB

#define  Lcd_A0_Pin						GPIO_Pin_1
#define  Lcd_A0_Port					GPIOB
#define  Lcd_A0_Clk						RCC_AHBPeriph_GPIOB  

#define  Lcd_Enb_Pin					GPIO_Pin_13
#define  Lcd_Enb_Port					GPIOB
#define  Lcd_Enb_Clk					RCC_AHBPeriph_GPIOB

#define  Lcd_Rst_Pin					GPIO_Pin_5
#define  Lcd_Rst_Port					GPIOC
#define  Lcd_Rst_Clk					RCC_AHBPeriph_GPIOC

#define  Lcd_BL_Pin						GPIO_Pin_0
#define  Lcd_BL_Port					GPIOB
#define  Lcd_BL_Clk						RCC_AHBPeriph_GPIOB

#define  Lcd_CS_Pin						GPIO_Pin_1
#define  Lcd_CS_Port					GPIOB
#define  Lcd_CS_Clk						RCC_AHBPeriph_GPIOB 

#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
