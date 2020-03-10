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
bool Setting_Mode = false;

uint32_t RTC_Second = 0;
uint8_t RTC_Saat;
uint8_t RTC_Gun;
uint8_t Dimmer;  // 0-100

#define I_BACKUP_ADR 0
#define COEFFICIENT 0.01
#define BANDRANGE  0.5 // derece

/*-----------------------------------------------------------*/
#define Ms(c)   pdMS_TO_TICKS(c)
/*-----------------------------------------------------------*/
volatile xSemaphoreHandle xMeasureSemaphore;
volatile xSemaphoreHandle xMutex_Display;
volatile xSemaphoreHandle xEncoderSemaphore;
volatile xSemaphoreHandle xButonSemaphore = {0};
volatile xSemaphoreHandle xBuzzerSemaphore;
volatile xSemaphoreHandle xRTCSemaphore;
volatile xQueueHandle xQueue;
volatile xTimerHandle BL_Timer;

static void vTask_Encoder(void *pvParameters);
static void vTask_Sensor(void *pvParameters);
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
void Set_Float_Backup(uint8_t adr, float *reg)
{
    uint8_t *ptr = (uint8_t *)reg;
    uint32_t temp;
    temp = *ptr++;
    temp |= *ptr++ << 8;
    temp |= *ptr++ << 16;
    temp |= *ptr++ << 24;
    RTC_WriteBackupRegister(adr, temp);
}
/***************************************************************************************************************************/
float Get_Float_Backup(uint8_t adr)
{
    float temp_f;
    uint8_t *ptr = (uint8_t *)&temp_f;
    uint32_t temp = RTC_ReadBackupRegister(adr);
    *ptr++ = temp;
    *ptr++ = temp >> 8;
    *ptr++ = temp >> 16;
    *ptr++ = temp >> 24;
    return temp_f;
}
/***************************************************************************************************************************/
void Backlight_Active(void)
{
    Gpio_Low(Lcd_BL_Port, Lcd_BL_Pin);
    xTimerStart(BL_Timer, portMAX_DELAY);
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
int main(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0x0250); // 6sn sonra yeniden yüklenmezse wdt reset atacak
    IWDG_ReloadCounter();
    IWDG_Enable();

    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        Set_Float_Backup(I_BACKUP_ADR, 0);
    }
    disableInterrupts();

    HW_Setup();

    Lcd_Pin_Init();
    Lcd_Init();

    xMutex_Display = xSemaphoreCreateMutex(); // Mutex -> LCD
    vSemaphoreCreateBinary(xMeasureSemaphore);      // RTC interrupt semaphore
    vSemaphoreCreateBinary(xEncoderSemaphore);    // encoder interrupt semaphore
    vSemaphoreCreateBinary(xButonSemaphore);      // buton interrupt semaphore
    vSemaphoreCreateBinary(xBuzzerSemaphore);      // Buzzer interrupt semaphore
    vSemaphoreCreateBinary(xRTCSemaphore);      // RTC interrupt semaphore

    BL_Timer = xTimerCreate("BL Timer", Ms(5000), pdTRUE, NULL, vBL_Timer);   // backlight timer

    /* Start the two tasks as described at the top of this file. */
    xTaskCreate(vTask_Sensor ,  "Task Sensor" , 512, NULL, 9, NULL);
    xTaskCreate(vTask_Encoder,  "Taks Encoder", 512, NULL, 5, NULL);
    xTaskCreate(vTask_Buton, "Taks Buton", 512, NULL, 5, NULL);
    xTaskCreate(vTask_Buzzer, "Buzzer", 256, NULL, 15, NULL);
    xTaskCreate(vTask_RTC, "RTC", 256, NULL, 6, NULL);

    Int_Eeprom_ReadStr(0, (uint8_t *)Buff, 2); // Eeprom Hafizadan AYAR degerini okunur
    Encoder_Value = Buff[0];
    Encoder_Value |= Buff[1] << 8;

    Int_Eeprom_ReadStr(2, (uint8_t *)&RTC_Saat, 1); // Eeprom Hafizadan saat degerini okunur
    Int_Eeprom_ReadStr(3, (uint8_t *)&RTC_Gun,  1); // Eeprom Hafizadan Gün  degerini okunur

    vTaskStartScheduler();

    for(;;);

}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/**************************************************************************************************************************/
static void vTask_Sensor(void *pvParameters)
{
		float Temperature;
		float Humidity;
		float Set;	
    float I = Get_Float_Backup(I_BACKUP_ADR);

    xSemaphoreTake(xMeasureSemaphore, portMAX_DELAY);

    for(;;)
    {
        xSemaphoreTake(xMeasureSemaphore, portMAX_DELAY);

        if(!SHT11_Oku(&Temperature, &Humidity))
        {
            if((Temperature > 99) || (Temperature < (-10)) || (Humidity > 99)) // hatali ölçüm var
            {
                continue;// yanlis isi alindi checksum yanlis olma ihtimali var
            }
            Set = (float)(Encoder_Value * 0.1);

            if(Set <= Temperature - BANDRANGE)
            {
                Dimmer = 0;
                I = 0;
            }
            else if(Set >= Temperature + BANDRANGE)
            {
                Dimmer = 99;
                I = 0;
            }
            else
            {
                //--------- PI calculate ---------
                I += (float)(Set - Temperature); // hata payi
                Set_Float_Backup(I_BACKUP_ADR, &I);
                Set = (float)((Set + BANDRANGE) - Temperature + (I * COEFFICIENT));
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

            xSemaphoreTake(xMutex_Display, portMAX_DELAY);
            sprintf(Buff, "%02.1f", Temperature);
            Lcd_Str(Alt1, 4, Buff);
            sprintf(Buff, "%2.0f", Humidity);
            Lcd_Str(Alt2, 6, Buff);
            sprintf(Buff, "%02d", Dimmer);
            Lcd_Str(Ust, 0, Buff);
            xSemaphoreGive(xMutex_Display);
            IWDG_ReloadCounter();
        }
        else if(!DHT11_Oku(&Temperature, &Humidity))
        {
            if((Temperature > 99) || (Temperature < (-10)) || (Humidity > 99)) // hatali ölçüm var
            {
                continue;// yanlis isi alindi checksum yanlis olma ihtimali var
            }

            Set = (float)(Encoder_Value * 0.1);

            if(Set <= Temperature - BANDRANGE)
            {
                Dimmer = 0;
                I = 0;
            }
            else if(Set >= Temperature + BANDRANGE)
            {
                Dimmer = 99;
                I = 0;
            }
            else
            {
                //--------- PI calculate ---------
                I += (float)(Set - Temperature); // hata payi
                Set_Float_Backup(I_BACKUP_ADR, &I);
                Set = (float)((Set + BANDRANGE) - Temperature + (I * COEFFICIENT));
                Dimmer = (uint8_t)(Set * 100);
                //---------- Result -------------
                if(Dimmer > 99)
                {
                    Dimmer = 99;
                }
            }
            xSemaphoreTake(xMutex_Display, portMAX_DELAY);
            sprintf(Buff, "%02.1f", Temperature);
            Lcd_Str(Alt1, 4, Buff);
            sprintf(Buff, "%2.0f", Humidity);
            Lcd_Str(Alt2, 6, Buff);
            sprintf(Buff, "%02d", Dimmer);
            Lcd_Str(Ust, 0, Buff);
            xSemaphoreGive(xMutex_Display);
            IWDG_ReloadCounter();
        }
        else
        {
            xSemaphoreTake(xMutex_Display, portMAX_DELAY);
            Dimmer = 50;
            Lcd_Str(Alt1, 4, "XX.X"); // sensor yok
            Lcd_Str(Alt2, 6, "XX");
            xSemaphoreGive(xMutex_Display);
        }
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Encoder(void *pvParameters)
{
    xSemaphoreTake(xEncoderSemaphore, portMAX_DELAY);

    for(;;)
    {
        xSemaphoreTake(xEncoderSemaphore, portMAX_DELAY);

        Backlight_Active();

        if(Setting_Mode == true)
        {
            xSemaphoreTake(xMutex_Display, portMAX_DELAY);
            Lcd_Underline_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Invert_Str(Alt2, 9, Buff);
            xSemaphoreGive(xMutex_Display);
        }
    }
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_Buton(void *pvParameters)
{
    xSemaphoreTake(xButonSemaphore, portMAX_DELAY);

    for(;;)
    {
        xSemaphoreTake(xButonSemaphore, portMAX_DELAY);

        Backlight_Active();

        vTaskDelay(Ms(50));// debounce

        Setting_Mode = !Setting_Mode;

        xSemaphoreTake(xMutex_Display, portMAX_DELAY);

        if(Setting_Mode == true)
        {
            TIM_Cmd(Encoder_Timer, ENABLE);
            Lcd_Str(Alt1, 0, "ISI:     AYAR");
            Lcd_Str(Alt2, 0, "NEM: %       ");
            Lcd_Underline_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Invert_Str(Alt2, 9, Buff);
        }
        else
        {
            TIM_Cmd(Encoder_Timer, DISABLE);
            Lcd_Str(Alt1, 9, "AYAR");
            sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
            Lcd_Str(Alt2, 9, Buff);
            taskENTER_CRITICAL();
            Int_Eeprom_WriteStr(0, (uint8_t *)&Encoder_Value, 2);
            taskEXIT_CRITICAL();
        }

        xSemaphoreGive(xMutex_Display);

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

    TIM_Cmd(Encoder_Timer, DISABLE);

    taskENTER_CRITICAL();
    Int_Eeprom_WriteStr(0, (uint8_t *)&Encoder_Value, 2);
    taskEXIT_CRITICAL();

    Setting_Mode = false;
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/
static void vTask_RTC(void *pvParameters)
{
    uint8_t Button_counter = 0;
    /* Remove compiler warning about unused parameter. */
    (void) pvParameters;

    for(;;)
    {
        xSemaphoreTake(xRTCSemaphore, portMAX_DELAY);  // 1sn RTC Wkup tick

        switch(RTC_Second++)
        {
            case 0:
                Backlight_Active();
                xSemaphoreTake(xMutex_Display, portMAX_DELAY);
                Lcd_Underline_Str(Alt1, 0, "COSKUN ERGAN");
                Lcd_Invert_Str(Alt2, 0,    "ISI-NEM V1.4");
                sprintf(Buff, "::%0.2d%0.2d", RTC_Gun, RTC_Saat);
                Display_String_Yaz(Buff);
                xSemaphoreGive(xMutex_Display);
                break;
            case 1:
                break;
            case 2:
                xSemaphoreTake(xMutex_Display, portMAX_DELAY);
                Lcd_Str(Alt1, 0, "ISI:     AYAR");
                Lcd_Str(Alt2, 0, "NEM: %       ");
                sprintf(Buff, "%02d.%d", Encoder_Value / 10, Encoder_Value % 10);
                Lcd_Str(Alt2, 9, Buff);
                xSemaphoreGive(xMutex_Display);
                xSemaphoreGive(xEncoderSemaphore);
                xSemaphoreGive(xMeasureSemaphore);
                break;
//            case 3600: // safety reset
//                RTC_Second = 0;
//                RTC_Saat++;
//                if(RTC_Saat >= 24)
//                {
//                    RTC_Saat = 0;
//                    RTC_Gun++;
//                }
//                taskENTER_CRITICAL();
//                Int_Eeprom_WriteStr(2, (uint8_t *)&RTC_Saat, 1);
//                Int_Eeprom_WriteStr(3, (uint8_t *)&RTC_Gun, 1);
//                Int_Eeprom_WriteStr(4, (uint8_t *)&I, 4); // 4-5-6-7 fload veri
//                while(1);	// saatlik reset
            default:
                xSemaphoreGive(xMeasureSemaphore);
                if(!Gpio_Get(ButonB_Port, ButonB_Pin) && (Button_counter++ > 5)) // long press sense
                {
                    RTC_Saat = 0;
                    RTC_Gun = 0;
                    RTC_Second = 0;
                    taskENTER_CRITICAL();
                    Int_Eeprom_WriteStr(2, (uint8_t *)&RTC_Saat, 1);
                    Int_Eeprom_WriteStr(3, (uint8_t *)&RTC_Gun, 1);
                    taskEXIT_CRITICAL();
                    xSemaphoreGive(xBuzzerSemaphore);
                }
                else
                {
                    Button_counter = 0;
                }
                break;
        }
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
        Gpio_Low(Buzzer_Port, Buzzer_Pin);

        xSemaphoreTake(xBuzzerSemaphore, Ms(50));

        Gpio_High(Buzzer_Port, Buzzer_Pin);

        xSemaphoreTake(xBuzzerSemaphore, portMAX_DELAY);
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

