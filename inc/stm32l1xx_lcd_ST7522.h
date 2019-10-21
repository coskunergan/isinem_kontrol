/**
  ******************************************************************************
  * @file    stm32l1xx__lcd_ST7522.h
  * @author  Coskun ERGAN
  * @version V1.0.3
  * @date    12-Ekim-2012
  * @brief   ST7522 �ipsetli Glcd K�t�panesi 
  *
	*  @verbatim
  ******************************************************************************
	*   Notlar;
	*
	*		- Bu k�t�pane Delay Fonksiyonunu kullanir Systick ile "DelayMs(100);" 
	*			karsiligi 100Ms ye kurulmalidir.
	*		- GPIO tan1mlamalarini kendisi yapar portlara clock verir.(ULP i�in dikkate aliniz).
	*		- Bu k�t�panede RST ve CS1 pinleri kullanilmaz. 
	*				RST: Glcd kari �zerinde pull-up ve kapasite ile turn on devre olusturup �alisir.
	*						 reset pini kartda bos birakilacaktir.
	*		    CS1: bu pin �ip se�imini yapar tek �ip oldugu icin s�rekli "0" da tutulmalidir.
	*				     Kart �zerinden pin GND'ye Baglanmalidir.
	*
	*					
	******************************************************************************
	*
	*    Kullanmadan �nce sirasiyla;
	*		
	*				Lcd_Pin_Init();  // lcd i�in gerekli pin,port,clk tanimlamalarini yapar.
	*				Init_Lcd();      // lcd kurulumunu yapar eger sonrasinda Lcd_Str fonksiyonu kullanilacak ise
	*														bu fonksiyon atlanabilir Lcd_Str icerisinde de init_lcd mevcuttur.
	*				Lcd_Str ("Merhaba");		// glcd ye string g�nderir ve ikonlari d�zenler.
	*				Display_String_Yaz ("123456");		// Display b�l�m�ne string g�nderir.
	*				Ikon_Durum(Ikaz_Ikon,Yak);  / ekrandaki ikonlar1 yakar s�nd�r�r.
	*				Lcd_Clear(Alt_Ekran_2Satir,Alt_Ekran_Baslangic,Alt_Ekran_Bitis); // girilen adres degerlerinin
	*																																						arasindaki pixelleri siler.
  *    k�sayol tr: ����������
	******************************************************************************
	*@endverbatim
	*****************************************************************************/
	
/* Includes ------------------------------------------------------------------*/	
#include "stm32l1xx.h"
#include "board_define.h"

#ifndef __STM32L1xx_LCD_ST7522_H
#define __STM32L1xx_LCD_ST7522_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup Selenoid valf Ac Kapat k�t�panesi
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


// �zel tanimlamalar
#define Ust 			1
#define Alt1      2                 // Lcd nin alttaki ekran�n 1. satirina ve ustteki ekrana yazmak i�in page adres
#define Alt2      3
#define duz       0
#define ters      1
#define  Lcd_BL_Off  GPIO_SetBits(Lcd_BL_Port,Lcd_BL_Pin);
#define  Lcd_BL_On   GPIO_ResetBits(Lcd_BL_Port,Lcd_BL_Pin);	 

// Ekran�n Page ve segment adresleri
#define Alt_Ekran_1Satir      0xB0                 // Lcd nin alttaki ekran�n 1. satirina ve ustteki ekrana yazmak i�in page adres
#define Alt_Ekran_2Satir      0xB1                 // Lcd nin 2. satirina yazmak i�in page adres
#define Alt_Ekran_Baslangic   16                   // birinci sat�r�n baslang�� segment adresi
#define Alt_Ekran_Bitis       96                   // birinci sat�r�n biti� segment adresi
#define Ust_Ekran             0xB0                 // Ustteki k���k ekrana yazmak i�in page adres
#define Ust_Ekran_Baslangic   0                    // Ustteki ekran�n baslang�� segment adresi
#define Ust_Ekran_Bitis       16                   // Ustteki ekran�n biti� segment adresi
#define Ikon_Ekran            0xB8                 // Ikonlara yazmak i�in page adresi
#define Ikon_Baslangic        11                   // Ikonlar�n baslang�� segment adresi
#define Ikon_Bitis            96                   // Ikonlar�n biti� segmen adresi
// Ikonlar�n Segment Adresleri
#define Vana_Ikon             15                   // Vana ikonunun segement adresi
#define Anahtar_Ikon          54                   // Anahtar ikonunun segement adresi
#define Ikaz_Ikon             63                   // Ikaz ikonunun segement adresi
#define TL_Ikon               95                   // TL ikonunun segement adresi
#define M3_Ikon               93                   // M3 ikonunun segement adresi
#define PilDis_Ikon           11                   // Pil D�� �er�eve ikonunun segement adresi
#define PilIc1_Ikon           13                   // Pil �� 1. sekme  ikonunun segement adresi
#define PilIc2_Ikon           14                   // Pil �� 2. sekme ikonunun segement adresi
#define PilIc3_Ikon           12                   // Pil �� 3. sekme ikonunun segement adresi
#define Nokta1_Ikon           70                   // Displaylar Aras� 1. Nokta ikonunun segement adresi
#define Nokta2_Ikon           80                   // Displaylar Aras� 2. Nokta ikonunun segement adresi
#define Cizgi_Ikon            94                   // TL M3 Aras� �izgi ikonunun segement adresi
#define Yak                   0x01                 // Ikonu yakmak veya s�nd�rmek i�in "Ikon_Durum(unsigned char ikon_segment,unsigned char durum)"
#define Sondur                0x00                 // durum k�sm�na yaz�l�r
// Ekran Secimi "Lcd_Str (unsigned char common,unsigned char segment,unsigned char *text,unsigned char text_size,unsigned char Ekran)"
// altprogram�nda Ekran k�sm�na yaz�l�r
#define Alt_Ekran_Secim       0x01                 // alttaki b�y�k ekran�n se�ilmesi
#define Ust_Ekran_Secim       0x00                 // �stteki k���k ekran�n se�ilmesi

void Lcd_Str_Big(const uint8_t *text);
void Lcd_Test(void);
void Lcd_Lp_Off(void);
void Lcd_Kontrast_Yenile(void);
void Lcd_Init(void);
void Lcd_Pin_Init(void);
void Komut_Gon(uint8_t komut);
void Veri_Gon(uint8_t ekran,uint8_t veri);
void Ikon_Durum(uint8_t ikon_segment,uint8_t durum);
void Display_Ikon_Durum(uint8_t ikon_segment,uint8_t durum);
void Lcd_Clear(uint8_t commons,uint8_t segments,uint8_t segments_sinir);
void Lcd_Center_Str(uint8_t common,const char *text);
void Lcd_Str(uint8_t common, uint8_t start_Adr, const char *text );
void Lcd_Invert_Str(uint8_t common, uint8_t start_Adr, const char *text );
void Lcd_Underline_Str(uint8_t common, uint8_t start_Adr, const char *text );
void Lcd_Antenna(uint8_t size);
void Display_Yaz(uint8_t rakam,uint8_t display);
void Display_String_Yaz(char *ptr);
void Lcd_Contrast(uint8_t value);

#ifdef __cplusplus
}
#endif
  
#endif /* __STM32L1xx_LCD_ST7522_H*/
