/**
  ******************************************************************************
  * @file    stm32l1xx__lcd_ST7522.h
  * @author  Coskun ERGAN
  * @version V1.0.3
  * @date    12-Ekim-2012
  * @brief   ST7522 Çipsetli Glcd Kütüpanesi 
  *
	*  @verbatim
  ******************************************************************************
	*   Notlar;
	*
	*		- Bu kütüpane Delay Fonksiyonunu kullanir Systick ile "DelayMs(100);" 
	*			karsiligi 100Ms ye kurulmalidir.
	*		- GPIO tan1mlamalarini kendisi yapar portlara clock verir.(ULP için dikkate aliniz).
	*		- Bu kütüpanede RST ve CS1 pinleri kullanilmaz. 
	*				RST: Glcd kari üzerinde pull-up ve kapasite ile turn on devre olusturup çalisir.
	*						 reset pini kartda bos birakilacaktir.
	*		    CS1: bu pin çip seçimini yapar tek çip oldugu icin sürekli "0" da tutulmalidir.
	*				     Kart üzerinden pin GND'ye Baglanmalidir.
	*
	*					
	******************************************************************************
	*
	*    Kullanmadan önce sirasiyla;
	*		
	*				Lcd_Pin_Init();  // lcd için gerekli pin,port,clk tanimlamalarini yapar.
	*				Init_Lcd();      // lcd kurulumunu yapar eger sonrasinda Lcd_Str fonksiyonu kullanilacak ise
	*														bu fonksiyon atlanabilir Lcd_Str icerisinde de init_lcd mevcuttur.
	*				Lcd_Str ("Merhaba");		// glcd ye string gönderir ve ikonlari düzenler.
	*				Display_String_Yaz ("123456");		// Display bölümüne string gönderir.
	*				Ikon_Durum(Ikaz_Ikon,Yak);  / ekrandaki ikonlar1 yakar söndürür.
	*				Lcd_Clear(Alt_Ekran_2Satir,Alt_Ekran_Baslangic,Alt_Ekran_Bitis); // girilen adres degerlerinin
	*																																						arasindaki pixelleri siler.
  *    kýsayol tr: ýÝÞþÇçÐðÜü
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

/** @addtogroup Selenoid valf Ac Kapat kütüpanesi
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


// özel tanimlamalar
#define Ust 			1
#define Alt1      2                 // Lcd nin alttaki ekranýn 1. satirina ve ustteki ekrana yazmak için page adres
#define Alt2      3
#define duz       0
#define ters      1
#define  Lcd_BL_Off  GPIO_SetBits(Lcd_BL_Port,Lcd_BL_Pin);
#define  Lcd_BL_On   GPIO_ResetBits(Lcd_BL_Port,Lcd_BL_Pin);	 

// Ekranýn Page ve segment adresleri
#define Alt_Ekran_1Satir      0xB0                 // Lcd nin alttaki ekranýn 1. satirina ve ustteki ekrana yazmak için page adres
#define Alt_Ekran_2Satir      0xB1                 // Lcd nin 2. satirina yazmak için page adres
#define Alt_Ekran_Baslangic   16                   // birinci satýrýn baslangýç segment adresi
#define Alt_Ekran_Bitis       96                   // birinci satýrýn bitiþ segment adresi
#define Ust_Ekran             0xB0                 // Ustteki küçük ekrana yazmak için page adres
#define Ust_Ekran_Baslangic   0                    // Ustteki ekranýn baslangýç segment adresi
#define Ust_Ekran_Bitis       16                   // Ustteki ekranýn bitiþ segment adresi
#define Ikon_Ekran            0xB8                 // Ikonlara yazmak için page adresi
#define Ikon_Baslangic        11                   // Ikonlarýn baslangýç segment adresi
#define Ikon_Bitis            96                   // Ikonlarýn bitiþ segmen adresi
// Ikonlarýn Segment Adresleri
#define Vana_Ikon             15                   // Vana ikonunun segement adresi
#define Anahtar_Ikon          54                   // Anahtar ikonunun segement adresi
#define Ikaz_Ikon             63                   // Ikaz ikonunun segement adresi
#define TL_Ikon               95                   // TL ikonunun segement adresi
#define M3_Ikon               93                   // M3 ikonunun segement adresi
#define PilDis_Ikon           11                   // Pil Dýþ Çerçeve ikonunun segement adresi
#define PilIc1_Ikon           13                   // Pil Ýç 1. sekme  ikonunun segement adresi
#define PilIc2_Ikon           14                   // Pil Ýç 2. sekme ikonunun segement adresi
#define PilIc3_Ikon           12                   // Pil Ýç 3. sekme ikonunun segement adresi
#define Nokta1_Ikon           70                   // Displaylar Arasý 1. Nokta ikonunun segement adresi
#define Nokta2_Ikon           80                   // Displaylar Arasý 2. Nokta ikonunun segement adresi
#define Cizgi_Ikon            94                   // TL M3 Arasý Çizgi ikonunun segement adresi
#define Yak                   0x01                 // Ikonu yakmak veya söndürmek için "Ikon_Durum(unsigned char ikon_segment,unsigned char durum)"
#define Sondur                0x00                 // durum kýsmýna yazýlýr
// Ekran Secimi "Lcd_Str (unsigned char common,unsigned char segment,unsigned char *text,unsigned char text_size,unsigned char Ekran)"
// altprogramýnda Ekran kýsmýna yazýlýr
#define Alt_Ekran_Secim       0x01                 // alttaki büyük ekranýn seçilmesi
#define Ust_Ekran_Secim       0x00                 // Üstteki küçük ekranýn seçilmesi

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
