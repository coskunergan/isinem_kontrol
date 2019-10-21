/*
  ******************************************************************************
  * @file    main.c
  * @author  Coskun ERGAN
  * @version V1.0.0
  * @date    06 Agust 2014
  * @brief   ISI-Nem Ölçüm Yazilimi
  ******************************************************************************
  *
*/

#include <stm32l1xx.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* ST library functions. */
#include "hw_init.h"
#include "stm32l1xx_lcd_ST7522.h"
#include "stm32l1xx_sht11.h"
#include "stm32l1xx_dht11.h"
#include "stm32l1xx_int_eeprom.h"
#include "stdio.h"

char Buff[32];
uint8_t DHT_Buffer[5];
bool Ayar_Modu;
bool Reset_Atildi;
float  Sht_Isi;
float  Sht_Nem;
float  Set;
float  P;
float  I;
float  D;
uint16_t Olculen_Isi;
uint16_t RTC_Saniye;
uint8_t RTC_Saat;
uint8_t RTC_Gun;
uint8_t Dimmer;  // 0-100

#define Integral_Carpani 0.01
#define Band_Genisligi  0.5 // derece
/*-----------------------------------------------------------*/
#define Ms(c)   pdMS_TO_TICKS(c)
/*-----------------------------------------------------------*/
volatile xSemaphoreHandle xMutex;
volatile xSemaphoreHandle xEncoderSemaphore;
volatile xSemaphoreHandle xButonSemaphore;
volatile xSemaphoreHandle xBuzzerSemaphore;
volatile xSemaphoreHandle xRTCSemaphore;
volatile xQueueHandle xQueue;
volatile xTimerHandle BL_Timer;

static void vTask_Encoder(void *pvParameters);
static void vTask_Sensor(void *pvParameters);
static void vTask_Backlight(void *pvParameters);
static void vBL_Timer(xTimerHandle xTimer);
static void vTask_Buton(void *pvParameters);
static void vTask_Buzzer(void *pvParameters);
static void vTask_RTC(void *pvParameters);
/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);
void vAssertCalled(unsigned long ulLine, const char *const pcFileName);
/***************************************************************************************************************************/
void DelayMs(uint32_t t)
{
    t *= 177; // ms/1mhz
    while(t--)
    {
        __NOP();
        __NOP();
    }
}
/***************************************************************************************************************************/
void DelayS(uint8_t t)
{
    while(t--)
    {
        DelayMs(1000);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
int main(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0x0150); // 3sn sonra yeniden yüklenmezse wdt reset atacak
    IWDG_ReloadCounter();
    IWDG_Enable();
		Reset_Atildi=FALSE;
	  if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
			Reset_Atildi=TRUE;
		}
    disableInterrupts();

    HW_Setup();

    Lcd_Pin_Init();
    Lcd_Init();
		
	  if(Reset_Atildi!=TRUE)
    {
			Gpio_Low(Lcd_BL_Port, Lcd_BL_Pin);
			Lcd_Underline_Str(Alt1, 0, "COSKUN ERGAN");
			Lcd_Invert_Str(Alt2, 0,    "ISI-NEM V1.3");
			IWDG_ReloadCounter();
			DelayS(2);
			IWDG_ReloadCounter();
		}
			
		Lcd_Str(Alt1, 0, "ISI:     AYAR");
		Lcd_Str(Alt2, 0, "NEM: %       ");

    xMutex = xSemaphoreCreateMutex(); // Mutex -> LCD
    vSemaphoreCreateBinary(xEncoderSemaphore);    // encoder interrupt semaphore
    vSemaphoreCreateBinary(xButonSemaphore);      // buton interrupt semaphore
    vSemaphoreCreateBinary(xBuzzerSemaphore);      // Buzzer interrupt semaphore
    vSemaphoreCreateBinary(xRTCSemaphore);      // RTC interrupt semaphore

    BL_Timer = xTimerCreate("BL Timer", Ms(5000), pdTRUE, NULL, vBL_Timer);   // backlight timer

    /* Start the two tasks as described at the top of this file. */
    xTaskCreate(vTask_Sensor ,  "Task Sensor" , 700, NULL, 1, NULL);
    xTaskCreate(vTask_Encoder,  "Taks Encoder", 700, NULL, 1, NULL);
    xTaskCreate(vTask_Backlight, "Taks Backlight", 700, NULL, 1, NULL);
    xTaskCreate(vTask_Buton, "Taks Buton", 700, NULL, 1, NULL);
    xTaskCreate(vTask_Buzzer, "Buzzer", 700, NULL, 1, NULL);
    xTaskCreate(vTask_RTC, "RTC", 700, NULL, 1, NULL);

    Int_Eeprom_ReadStr(0, (uint8_t *)Buff, 2); // Eeprom Hafizadan AYAR degerini okunur
    Encoder_Value = Buff[0];
    Encoder_Value |= Buff[1] << 8;

    Int_Eeprom_ReadStr(2, (uint8_t *)&RTC_Saat, 1); // Eeprom Hafizadan saat degerini okunur
    Int_Eeprom_ReadStr(3, (uint8_t *)&RTC_Gun,  1); // Eeprom Hafizadan Gün  degerini okunur
    Int_Eeprom_ReadStr(4, (uint8_t *)&I, 4);
		Ayar_Modu = TRUE;
		
    vTaskStartScheduler();

    for(;;);

}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Sensor(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(Ms(500));

        if(!SHT11_Oku(&Sht_Isi, &Sht_Nem))
        {
            if((Sht_Isi > 99) | (Sht_Isi < (-10)) | (Sht_Nem > 99)) // hatali ölçüm var
            {
                continue;// yanlis isi alindi checksum yanlis olma ihtimali var
            }
            Set = (float)(Encoder_Value * 0.1);

            if(Set <= Sht_Isi - Band_Genisligi)
            {
                Dimmer = 0;
                I = 0;
            }
            else if(Set >= Sht_Isi + Band_Genisligi)
            {
                Dimmer = 99;
                I = 0;
            }
            else
            {
                //--------- PI calculate ---------
                I += (float)(Set - Sht_Isi); // hata payi
                Set = (float)((Set + Band_Genisligi) - Sht_Isi + (I * Integral_Carpani));
                if(Set < 0)
                {
                    Dimmer = 0;
                }
                else
                {
                    Dimmer = (uint8_t)(Set * 100);
                }
                //---------- Result -------------
                if(Dimmer > 99)
                {
                    Dimmer = 99;
                }
            }

            xSemaphoreTake(xMutex, portMAX_DELAY);
            sprintf(Buff, "%02.1f", Sht_Isi);
            Lcd_Str(Alt1, 4, Buff);
            sprintf(Buff, "%2.0f", Sht_Nem);
            Lcd_Str(Alt2, 6, Buff);
            sprintf(Buff, "%02d", Dimmer);
            Lcd_Str(Ust, 0, Buff);
            xSemaphoreGive(xMutex);
            IWDG_ReloadCounter();
        }
        else if(!DHT11_Oku(&Sht_Isi, &Sht_Nem))
        {

            Set = (float)(Encoder_Value * 0.1);

            if(Set <= Sht_Isi - Band_Genisligi)
            {
                Dimmer = 0;
                I = 0;
            }
            else if(Set >= Sht_Isi + Band_Genisligi)
            {
                Dimmer = 99;
                I = 0;
            }
            else
            {
                //--------- PI calculate ---------
                I += (float)(Set - Sht_Isi); // hata payi
                Set = (float)((Set + Band_Genisligi) - Sht_Isi + (I * Integral_Carpani));
                Dimmer = (uint8_t)(Set * 100);
                //---------- Result -------------
                if(Dimmer > 99)
                {
                    Dimmer = 99;
                }
            }
            xSemaphoreTake(xMutex, portMAX_DELAY);
            sprintf(Buff, "%02.1f", Sht_Isi);
            Lcd_Str(Alt1, 4, Buff);
            sprintf(Buff, "%2.0f", Sht_Nem);
            Lcd_Str(Alt2, 6, Buff);
            sprintf(Buff, "%02d", Dimmer);
            Lcd_Str(Ust, 0, Buff);
            xSemaphoreGive(xMutex);
            IWDG_ReloadCounter();
        }
        else
        {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            Dimmer = 50;
            Lcd_Str(Alt1, 4, "XX.X"); // sensor yok
            Lcd_Str(Alt2, 6, "XX");
            xSemaphoreGive(xMutex);
        }
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Encoder(void *pvParameters)
{
    for(;;)
    {
        xSemaphoreTake(xEncoderSemaphore, portMAX_DELAY);
			
				if(Reset_Atildi==TRUE)			
				{
					continue;
				}

        xSemaphoreTake(xMutex, portMAX_DELAY);
        if(Ayar_Modu == TRUE)
        {
            Lcd_Underline_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Invert_Str(Alt2, 9, Buff);
        }
        else
        {
            Lcd_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Str(Alt2, 9, Buff);
        }
        xSemaphoreGive(xMutex);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Backlight(void *pvParameters)
{
    for(;;)
    {
        xSemaphoreTake(xEncoderSemaphore, portMAX_DELAY);
			  if(Reset_Atildi==TRUE)
				{
					continue;
				}			
        Gpio_Low(Lcd_BL_Port, Lcd_BL_Pin);

        xTimerStart(BL_Timer, portMAX_DELAY);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Buton(void *pvParameters)
{
    for(;;)
    {
        xSemaphoreTake(xButonSemaphore, portMAX_DELAY);
			
        vTaskDelay(Ms(50));

			  if(Reset_Atildi==FALSE)
				{					
					Gpio_Low(Lcd_BL_Port, Lcd_BL_Pin);
				}

        xTimerStart(BL_Timer, portMAX_DELAY);

        Ayar_Modu = !Ayar_Modu;

        xSemaphoreTake(xMutex, portMAX_DELAY);
        if(Ayar_Modu == TRUE)
        {
            TIM_Cmd(Encoder_Timer, ENABLE);
            Lcd_Underline_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Invert_Str(Alt2, 9, Buff);
        }
        else
        {
            taskENTER_CRITICAL();
            Int_Eeprom_WriteStr(0, (uint8_t *)&Encoder_Value, 2);
            taskEXIT_CRITICAL();
            TIM_Cmd(Encoder_Timer, DISABLE);
            Lcd_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Str(Alt2, 9, Buff);
        }

        xSemaphoreGive(xMutex);
        xSemaphoreGive(xBuzzerSemaphore);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
void vBL_Timer(xTimerHandle xTimer)
{
    Gpio_High(Lcd_BL_Port, Lcd_BL_Pin);

    xTimerStop(BL_Timer, portMAX_DELAY);
    Ayar_Modu = FALSE;
    TIM_Cmd(Encoder_Timer, DISABLE);

    if(!Gpio_Get(ButonB_Port, ButonB_Pin)) // 5n uzun basilmis ise
    {
        RTC_Saat = 0;
        RTC_Gun = 0;
        RTC_Saniye = 0;
        taskENTER_CRITICAL();
        Int_Eeprom_WriteStr(2, (uint8_t *)&RTC_Saat, 1);
        Int_Eeprom_WriteStr(3, (uint8_t *)&RTC_Gun, 1);
        taskEXIT_CRITICAL();
        xSemaphoreGive(xBuzzerSemaphore);
        xSemaphoreGive(xRTCSemaphore);
    }
    else
    {
        if(Ayar_Modu == TRUE)
        {
            taskENTER_CRITICAL();
            Int_Eeprom_WriteStr(0, (uint8_t *)&Encoder_Value, 2);
            taskEXIT_CRITICAL();
        }
        xSemaphoreGive(xEncoderSemaphore);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_RTC(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    (void) pvParameters;

    for(;;)
    {
        xSemaphoreTake(xRTCSemaphore, portMAX_DELAY);  // 1sn RTC Wkup tick

        RTC_Saniye++;
        if(RTC_Saniye == 2) // reset at1ld1ktan 2 sn sonra temziler ki ses c1kmas1n isik yanmasin 
        {			
				Reset_Atildi=FALSE;
				}

        if(RTC_Saniye >= 3600)
        {
            RTC_Saniye = 0;
            RTC_Saat++;
            if(RTC_Saat >= 24)
            {
                RTC_Saat = 0;
                RTC_Gun++;
            }
            taskENTER_CRITICAL();
            Int_Eeprom_WriteStr(2, (uint8_t *)&RTC_Saat, 1);
            Int_Eeprom_WriteStr(3, (uint8_t *)&RTC_Gun, 1);
            Int_Eeprom_WriteStr(4, (uint8_t *)&I, 4); // 4-5-6-7 fload veri
            while(1);	// saatlik reset					
        }
        xSemaphoreTake(xMutex, portMAX_DELAY);
        sprintf(Buff, "::%0.2d%0.2d", RTC_Gun, RTC_Saat);
        Display_String_Yaz(Buff);
        xSemaphoreGive(xMutex);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Buzzer(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    (void) pvParameters;

    for(;;)
    {
				xSemaphoreTake(xBuzzerSemaphore, portMAX_DELAY);
			  if(Reset_Atildi==TRUE)
				{
					continue;
				}			
        Gpio_Low(Buzzer_Port, Buzzer_Pin);
        vTaskDelay(Ms(50));
        Gpio_High(Buzzer_Port, Buzzer_Pin);
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
void vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for(;;);
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
void vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for(;;);
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
void vApplicationTickHook(void)
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
void vAssertCalled(unsigned long ulLine, const char *const pcFileName)
{
    volatile unsigned long ulSetToNonZeroInDebuggerToContinue = 0;

    /* Parameters are not used. */
    (void) ulLine;
    (void) pcFileName;

    taskENTER_CRITICAL();
    {
        while(ulSetToNonZeroInDebuggerToContinue == 0)
        {
            /* Use the debugger to set ulSetToNonZeroInDebuggerToContinue to a
            non zero value to step out of this function to the point that raised
            this assert(). */
            __asm volatile("NOP");
            __asm volatile("NOP");
        }
    }
    taskEXIT_CRITICAL();
}

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/

