/**
  ******************************************************************************
  * @file    stm32l1xx_lcd_ST7522.c
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
	*		- Bu kütüpanede CS1 pini kullanilmaz.
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
	*				Lcd_Str (Alt1,"Merhaba");		// glcd ye string gönderir ve ikonlari düzenler.
	*				Display_String_Yaz ("123456");		// Display bölümüne string gönderir.
	*				Ikon_Durum(Ikaz_Ikon,Yak);  / ekrandaki ikonlar1 yakar söndürür.
	*				Lcd_Clear(Alt_Ekran_2Satir,Alt_Ekran_Baslangic,Alt_Ekran_Bitis); // girilen adres degerlerinin
	*																																						arasindaki pixelleri siler.
  *  kisayol tr: isgöçü ISGÖÇÜ
	*	 kisayol BL: GPIO_ResetBits(Lcd_BL_Port,Lcd_BL_Pin);
	******************************************************************************
	*@endverbatim
	*****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_lcd_ST7522.h"
#include "stm32l1xx_lcd_tablo.h"

/** @addtogroup
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

uint16_t Battery;

//------------------------------------------------------------------------------
//  LCD'nin Test modu
//------------------------------------------------------------------------------
void Lcd_Test(void)
{
    uint8_t j;
    uint8_t send_segment, segment = Ust_Ekran_Baslangic;

    if(GPIO_ReadOutputDataBit(Lcd_Enb_Port, Lcd_Enb_Pin)) // Lcd On konumunda degil ise Lcd yi aç
    {
        Lcd_Init();
        Komut_Gon(0xAF);    // LCD On komutu
        GPIO_ResetBits(Lcd_BL_Port, Lcd_BL_Pin);        //  Backlight aktif
    }

    Display_String_Yaz("888888");

    Ikon_Durum(PilIc1_Ikon, Yak);
    Ikon_Durum(PilIc2_Ikon, Yak);
    Ikon_Durum(PilIc3_Ikon, Yak);
    Ikon_Durum(PilDis_Ikon, Yak);
    Ikon_Durum(Nokta1_Ikon, Yak);
    Ikon_Durum(Nokta2_Ikon, Yak);
    Ikon_Durum(Anahtar_Ikon, Yak);
    Ikon_Durum(Vana_Ikon, Yak);

    Ikon_Durum(TL_Ikon, Yak);
    Ikon_Durum(M3_Ikon, Yak);
    Ikon_Durum(Cizgi_Ikon, Yak);
    Ikon_Durum(Ikaz_Ikon, Yak);

    Komut_Gon(Ust_Ekran);
    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    for(j = 0; j < 96; j++)
    {
        Veri_Gon(0, 0xFF);
    }

    Komut_Gon(Alt_Ekran_2Satir);
    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    for(j = 0; j < 96; j++)
    {
        Veri_Gon(0, 0xFF);
    }

}
//------------------------------------------------------------------------------
//  LCD'nin Pinlerinin low power moda sokuldugu alt program
//------------------------------------------------------------------------------
void Lcd_Lp_Off(void)
{
    Lcd_Enb_Port->BSRRL = Lcd_Enb_Pin;
    Lcd_BL_Port->BSRRL = Lcd_BL_Pin;

    Lcd_Si_Port->BSRRH = Lcd_Si_Pin;
    Lcd_Scl_Port->BSRRH = Lcd_Scl_Pin;
    Lcd_A0_Port->BSRRH = Lcd_A0_Pin;
    Lcd_Rst_Port->BSRRH = Lcd_Rst_Pin;
}
//------------------------------------------------------------------------------
//  LCD ye ait pinin kuruldugu alt program
//------------------------------------------------------------------------------
void The_Pin_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t GPIO_clk)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(GPIO_clk , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}
//------------------------------------------------------------------------------
//  LCD' pinlerinin kuruldugu alt program
//------------------------------------------------------------------------------
void Lcd_Pin_Init(void)
{
    The_Pin_Init(Lcd_Scl_Port, Lcd_Scl_Pin, Lcd_Scl_Clk);
    The_Pin_Init(Lcd_Si_Port, Lcd_Si_Pin, Lcd_Si_Clk);
    The_Pin_Init(Lcd_A0_Port, Lcd_A0_Pin, Lcd_A0_Clk);
    The_Pin_Init(Lcd_Enb_Port, Lcd_Enb_Pin, Lcd_Enb_Clk);
    The_Pin_Init(Lcd_Rst_Port, Lcd_Rst_Pin, Lcd_Rst_Clk);
    The_Pin_Init(Lcd_BL_Port, Lcd_BL_Pin, Lcd_BL_Clk);
    The_Pin_Init(Lcd_CS_Port, Lcd_CS_Pin, Lcd_CS_Clk);	//<<<<<< Bu pin hem low
    GPIO_SetBits(Lcd_Enb_Port, Lcd_Enb_Pin);
    GPIO_SetBits(Lcd_Rst_Port, Lcd_Rst_Pin);
    GPIO_SetBits(Lcd_BL_Port, Lcd_BL_Pin);
}
//------------------------------------------------------------------------------
//  LCD'ye 1 Bytlýk Komut Gönderen Alt program
//  Altprograma giren argümanlar
//  komut   = Lcd ye gönderilecek komut. MSB --> LSB dogru bit bit gönderilir
//------------------------------------------------------------------------------
void Komut_Gon(uint8_t komut)
{
    uint8_t i, bit = 0x80;
    GPIO_ResetBits(Lcd_A0_Port , Lcd_A0_Pin);     // A0   --> 0
    for(i = 0; i < 8; i++)
    {
        GPIO_ResetBits(Lcd_Si_Port , Lcd_Scl_Pin);    // Scl  --> 0
        if((komut & bit) == bit)
        {
            GPIO_SetBits(Lcd_Si_Port , Lcd_Si_Pin);    // SI   --> 1
        }
        else
        {
            GPIO_ResetBits(Lcd_Scl_Port , Lcd_Si_Pin);    // SI   --> 0
        }
        GPIO_SetBits(Lcd_Scl_Port , Lcd_Scl_Pin);     // SCL  --> 1
        bit >>= 1;
    }
}
//------------------------------------------------------------------------------
//  LCD'nin Alt Ekranýna veya Üst Ekranýna 1 Bytlýk karakter Gönderen Alt program
//  Altprograma giren argümanlar
//  ekrana    = Verilerin nasýl gönderilecegini seçmek için kullanýlýr.Alttaki büyük ekrana
//  gönderilecekse veriler LSB --> MSB ye dogru. Üstteki küçük ekrana gönderilecekse
//  veriler MSB --> LSB ye dogru gönderilir.
//  ekran     = 1 ise alttaki büyük ekrana veri yazýlacak demektir
//  ekran     = 0 ise üstteki küçük ekrana veri yazýlacak demektir
//  veri      = Lcd ye gönderilecek karakter
//------------------------------------------------------------------------------
void Veri_Gon(uint8_t ekran, uint8_t veri) // ekran altaki buyuk ekranamý yoksa ustteki kucuk
{
    // ekranamý yazýlacagýný secer.uste gonderirken byt bilgisi tersden gonderilmeli
    uint8_t i, bit;
    bit = 0x80;               // ekran = 0 ise veriyi MSB--> LSB ye dogru gönder
    if(ekran == 1)
    {
        bit = 0x01;    // ekran = 1 ise veriyi LSB--> MSB ye dogru gönder
    }
    GPIO_SetBits(Lcd_A0_Port , Lcd_A0_Pin);     // A0   --> 1
    for(i = 0; i < 8; i++)
    {
        GPIO_ResetBits(Lcd_Si_Port , Lcd_Scl_Pin);    // Scl  --> 0
        if((veri & bit) == bit)
        {
            GPIO_SetBits(Lcd_Si_Port , Lcd_Si_Pin);    // SI   --> 1
        }
        else
        {
            GPIO_ResetBits(Lcd_Scl_Port , Lcd_Si_Pin);    // SI   --> 0
        }
        GPIO_SetBits(Lcd_Scl_Port , Lcd_Scl_Pin);     // SCL  --> 1
        if(ekran == 1)
        {
            bit <<= 1;
        }
        else
        {
            bit >>= 1;
        }
    }
}
//------------------------------------------------------------------------------
//  LCD'deki Bir Ikonu Yakan/Söndüren Alt program
//  Altprograma giren argümanlar
//  ikon_segment  = Lcd deki yakýlacak veya söndürülecek ikonun segment adresi
//  durum         =1 ise ikon yakýlýr , 0 ise söndürülür.
//------------------------------------------------------------------------------
void Ikon_Durum(uint8_t ikon_segment, uint8_t durum)
{
    Komut_Gon(Ikon_Ekran);  // common olarak ikon page adersinin seçilmesi
    Komut_Gon((ikon_segment & 0x0F));
    ikon_segment = (ikon_segment & 0xF0);
    ikon_segment >>= 4;
    ikon_segment |= 0x10;
    Komut_Gon(ikon_segment);
    Veri_Gon(0, durum);
}
//------------------------------------------------------------------------------
//  LCD'deki Display Cubuklarýndan birini yakmak veya söndürmek için
//  Tarihi_Lcd_Str altprogramýnda kullanýlýr
//  Altprograma giren argümanlar
//  ikon_segment  = Lcd deki yakýlacak veya söndürülecek ikonun segment adresi
//  durum         =1 ise ikon yakýlýr , 0 ise söndürülür.
//------------------------------------------------------------------------------
void Display_Ikon_Durum(uint8_t ikon_segment, uint8_t durum)
{
    Komut_Gon((ikon_segment & 0x0F));
    ikon_segment = (ikon_segment & 0xF0);
    ikon_segment >>= 4;
    ikon_segment |= 0x10;
    Komut_Gon(ikon_segment);
    Veri_Gon(0, durum);
}
//------------------------------------------------------------------------------
//  LCD'nin Secilen Kýsmýný Temizleyen  Alt program
//  Altprograma giren argümanlar
//  commons         = Lcd'nin hangi page adresinin temizlenecegini belirtir
//  segmnets        = Page adresinin kacýncý segement adresinden baslanýlacagý bilgisi
//  segment_sinir   = segment adreslerinden nereye kadar devam edilecegi bilgisi
//------------------------------------------------------------------------------
void Lcd_Clear(uint8_t commons, uint8_t segments, uint8_t segments_sinir)
{
    uint8_t send_segment;
    // commons 0 'dan commons 7 'e kadar secilirse page adress=0xB0 büyük ekran birinci satýr ve küçük ekran
    // commons 8 'dan commons 16 'e kadar secilirse Lcd_Clear=0xB1 ikinci satir
    // commons 17 'den sonrasý Lcd_Clear=0xB8  ikonlarýn satirý
    // ust taraftaki kucuk satirin adresi 0 dan baslamalý

    if(!GPIO_ReadOutputDataBit(Lcd_Enb_Port, Lcd_Enb_Pin)) // Lcd On konumunda  ise
    {
        Komut_Gon(commons);

        do
        {
            send_segment = (segments & 0x0F);
            Komut_Gon(send_segment);
            send_segment = (segments & 0xF0);
            send_segment = send_segment / 16;
            send_segment |= 0x10;
            send_segment &= 0x1F;
            Komut_Gon(send_segment);
            Veri_Gon(1, 0x00);
            segments++;
            if((segments == 16) && (commons == Ikon_Ekran)) // Ikonlar temizlenecekse adres 16dan sonra 47 kadar ikon olmadýgý için
            {
                segments = 47;    // aradaki adrs degerleri atlanýr
            }
        }
        while(segments < segments_sinir);
    }
}
//------------------------------------------------------------------------------
//  LCD ye sabit contrast degeri giren alt program
//------------------------------------------------------------------------------
void Lcd_Contrast(uint8_t value)
{
    Komut_Gon(0x81);            // LCd contranst register
    Komut_Gon(value);
}
//------------------------------------------------------------------------------
//  LCD'ye battarty voltajina ayarli contrast degeri giren alt program
//------------------------------------------------------------------------------
void Lcd_Kontrast_Yenile(void)
{
    Komut_Gon(0x81);            // LCd contranst register

    if(Battery > 345)
    {
        Komut_Gon(0x00);
    }
    else if(Battery > 340)
    {
        Komut_Gon(0x01);
    }
    else if(Battery > 335)
    {
        Komut_Gon(0x02);
    }
    else if(Battery > 325)
    {
        Komut_Gon(0x04);
    }
    else if(Battery > 320)
    {
        Komut_Gon(0x08);
    }
    else if(Battery > 315)
    {
        Komut_Gon(0x10);
    }
    else
    {
        Komut_Gon(0x12);
    }

}
//------------------------------------------------------------------------------
//  LCD'nin Ayarlarýnýn Yapýldýgý  Alt program
//------------------------------------------------------------------------------
void Lcd_Init(void)
{
    uint8_t *ptr = (uint8_t *)Lcd_Komutlari;
    uint8_t t = 0;

    GPIO_ResetBits(Lcd_Enb_Port, Lcd_Enb_Pin);
    GPIO_ResetBits(Lcd_Rst_Port, Lcd_Rst_Pin);
    while(--t); // ~1mS
    GPIO_SetBits(Lcd_Rst_Port, Lcd_Rst_Pin);
    while(--t); // ~1mS

    do
    {
        Komut_Gon(*ptr);      // Lcd initialize komutlarýný gönder
        ptr++;
    }
    while(*ptr != 0xFF);

    Lcd_Contrast(0x8);
    while(--t); // ~1mS
    Komut_Gon(0xAF);    // LCD On komutu

  //  GPIO_ResetBits(Lcd_BL_Port, Lcd_BL_Pin);        //  Backlight aktif

    Lcd_Clear(Alt_Ekran_1Satir, Ust_Ekran_Baslangic, Alt_Ekran_Bitis); // 0xB0 page adresini temizle (Ust Ekran + Alt Ekran 1. Satýr)
    Lcd_Clear(Alt_Ekran_2Satir, Alt_Ekran_Baslangic, Alt_Ekran_Bitis); // 0xB1 page adresini temizle
    Lcd_Clear(Ikon_Ekran, Ikon_Baslangic, Ikon_Bitis);               // TL - Çizgi -M3 ikonlarýný temizle
}
//------------------------------------------------------------------------------
//  Tabloya Eklenen Türkçe Karakterlerin Görüntülenebilmesi Ýçin  Alt program
//  Altprograma giren argümanlar
//  karakter    = yazýlacak türkçe karakterin Ascii olarak gelen numarasý
//  Altprogramdan dönen argümanlar
//  cevrim      = yazýlacak türkçe karakterin bizim tablomuzdaki sýra numarasý
//------------------------------------------------------------------------------
char TurkceCevir(char karakter)
{
    char cevrim = 0;

    switch(karakter)
    {
        case 0xE7:
            cevrim = 0x7F;
            break;    // ç
        case 0xF0:
            cevrim = 0x80;
            break;    // ð
        case 0xFD:
            cevrim = 0x81;
            break;    // ý
        case 0xF6:
            cevrim = 0x82;
            break;    // ö
        case 0xFE:
            cevrim = 0x83;
            break;    // þ
        case 0xFC:
            cevrim = 0x84;
            break;    // ü
        case 0xC7:
            cevrim = 0x85;
            break;    // Ç
        case 0xD0:
            cevrim = 0x86;
            break;    // Ð
        case 0xDD:
            cevrim = 0x87;
            break;    // Ý
        case 0xD6:
            cevrim = 0x88;
            break;    // Ö
        case 0xDE:
            cevrim = 0x89;
            break;    // Þ
        case 0xDC:
            cevrim = 0x8A;
            break;    // Ü
        case 0xDF:
            cevrim = 0x8B;
            break;    // m3
        case 0xE0:
            cevrim = 0x8C;
            break;    // Su yönü ters oku
        case 0xE1:
            cevrim = 0x8D;
            break;    // Su yönü dogru oku
        case 0xE2:
            cevrim = 0x8E;
            break;    // Rf iþareti
    }
    return cevrim;
}
//------------------------------------------------------------------------------
void Lcd_Center_Str(uint8_t common, const char *text)
{
    uint8_t j, i;
    char buff[15] = "              ";
    uint8_t buff2[15];

    for(i = 0; i < 15; i++)
    {
        buff2[i] = *text;
        text++;
        if(buff2[i] == 0)
        {
            break;
        }
    }

    i = (14 - i) / 2;

    for(j = 0; j < 15; j++)
    {
        if(j >= i)
        {
            buff[j] = buff2[j - i];
        }
        if(buff[j] == 0)
        {
            break;
        }
    }
    buff[j] = ' ';
    for(; j < 15; j++)
    {
        buff[j] = ' ';
    }
    Lcd_Str(common, 0, buff);

}
//------------------------------------------------------------------------------
//  LCD'nin Secilen Kýsmýna Gönderilen Texti Yazan  Alt program
//  Altprograma giren argümanlar
//  commons       = Lcd'nin hangi page adresine yazýlacagýný belirtir
//  start_Adr     = Page adresinin kaçinci karakter adresinden baslanýlacagý bilgisi
//  text          = Ekrana yazýlacka olan tex bilgisi.
//------------------------------------------------------------------------------
void Lcd_Str(uint8_t common, uint8_t start_Adr, const char *text)
{
    uint8_t i = 0, j, send_segment, ekran = 0, segment = 0, say = 0;

    if(common == 1)
    {
        ekran = 1;
        Komut_Gon(Ust_Ekran);
        segment = Ust_Ekran_Baslangic + start_Adr * 6;
        say = 9;
    }
    else if(common == 2)
    {
        ekran = 0;
        Komut_Gon(Alt_Ekran_1Satir);
        segment = Alt_Ekran_Baslangic + start_Adr * 6;
    }
    else if(common == 3)
    {
        ekran = 0;
        Komut_Gon(Alt_Ekran_2Satir);
        segment = Alt_Ekran_Baslangic + start_Adr * 6;
    }

    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    Veri_Gon(ekran, 0);

    while(*text != '\0')
    {
        say++;
        j = *text;
        text++;
        if(j == 0)
        {
            j = 0x20;
        }
        if(126 < j)
        {
            j = TurkceCevir(j);
        }
        j -= 0x20;
        for(i = 0; i < 6; i++)
        {
            Veri_Gon(ekran, Karakter_Tablo[j][i]);    // yazilacak verileri gonder
        }
        if(say > 12)
        {
            break;
        }
    }

}
//-------------------------------------------------
//  LCD'nin Secilen Kýsmýna Gönderilen Texti Yazan  Alt program
//  Altprograma giren argümanlar
//  commons       = Lcd'nin hangi page adresine yazýlacagýný belirtir
//  start_Adr     = Page adresinin kaçinci karakter adresinden baslanýlacagý bilgisi
//  text          = Ekrana yazýlacka olan tex bilgisi.
//------------------------------------------------------------------------------
void Lcd_Invert_Str(uint8_t common, uint8_t start_Adr, const char *text)
{

    uint8_t i = 0, j, send_segment, ekran = 0, segment = 0, say = 0;

    if(common == 1)
    {
        ekran = 1;
        Komut_Gon(Ust_Ekran);
        segment = Ust_Ekran_Baslangic + start_Adr * 6;
        say = 9;
    }
    else if(common == 2)
    {
        ekran = 0;
        Komut_Gon(Alt_Ekran_1Satir);
        segment = Alt_Ekran_Baslangic + start_Adr * 6;
    }
    else if(common == 3)
    {
        ekran = 0;
        Komut_Gon(Alt_Ekran_2Satir);
        segment = Alt_Ekran_Baslangic + start_Adr * 6;
    }

    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    Veri_Gon(ekran, 0xFF);
    while(*text != '\0')
    {
        say++;
        j = *text;
        text++;
        if(j == 0)
        {
            j = 0x20;
        }
        if(126 < j)
        {
            j = TurkceCevir(j);
        }
        j -= 0x20;
        for(i = 0; i < 6; i++)
        {
            Veri_Gon(ekran, ~Karakter_Tablo[j][i]);    // yazilacak verileri gonder
        }
        if(say > 12)
        {
            break;
        }
    }
}
//------------------------------------------------------------------------------
//  LCD'nin Secilen Kýsmýna Gönderilen Texti Yazan  Alt program
//  Altprograma giren argümanlar
//  commons       = Lcd'nin hangi page adresine yazýlacagýný belirtir
//  start_Adr     = Page adresinin kaçinci karakter adresinden baslanýlacagý bilgisi
//  text          = Ekrana yazýlacka olan tex bilgisi.
//------------------------------------------------------------------------------
void Lcd_Underline_Str(uint8_t common, uint8_t start_Adr, const char *text)
{
    uint8_t i = 0, j, send_segment, ekran = 0, segment = 0, say = 0;

    if(common == 1)
    {
        ekran = 1;
        Komut_Gon(Ust_Ekran);
        segment = Ust_Ekran_Baslangic + start_Adr * 6;
        say = 9;
    }
    else if(common == 2)
    {
        ekran = 0;
        Komut_Gon(Alt_Ekran_1Satir);
        segment = Alt_Ekran_Baslangic + start_Adr * 6;
    }
    else if(common == 3)
    {
        ekran = 0;
        Komut_Gon(Alt_Ekran_2Satir);
        segment = Alt_Ekran_Baslangic + start_Adr * 6;
    }

    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    Veri_Gon(ekran, 0x80);

    while(*text != '\0')
    {
        say++;
        j = *text;
        text++;
        if(j == 0)
        {
            j = 0x20;
        }
        if(126 < j)
        {
            j = TurkceCevir(j);
        }
        j -= 0x20;
        for(i = 0; i < 6; i++)
        {
            Veri_Gon(ekran, Karakter_Tablo[j][i] | 0x80);  // yazilacak verileri gonder
        }
        if(say > 12)
        {
            break;
        }
    }

}
//-------------------------------------------------
void Lcd_Str_Big(const uint8_t *text)
{
    uint8_t i, j, send_segment, segment, say;

    segment = 0;
    say = 0;
    Komut_Gon(Alt_Ekran_1Satir);
    segment = Alt_Ekran_Baslangic;

    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    while(1)
    {
        say++;
        j = *text;
        text++;
        if(j == 'H')
        {
            j = 15;
        }
        else if(j == 'A')
        {
            j = 16;
        }
        else if(j == 'T')
        {
            j = 17;
        }
        else if(j == 'B')
        {
            j = 18;
        }
        else if(j == 'Y')
        {
            j = 19;
        }
        else if(j == 'L')
        {
            j = 20;
        }
        else if(j == 'N')
        {
            j = 21;
        }
        else
        {
            j -= 0x2B;
        }
        for(i = 0; i < 10; i++)
            if(j < 22)
            {
                Veri_Gon(1, Rakam_Tablo_Big_Ust[j][i]);    // yazilacak verileri gonder
            }
            else
            {
                Veri_Gon(1, 0);
            }
        if(say > 12)
        {
            break;
        }
    }

    segment = 0;
    say = 0;
    Komut_Gon(Alt_Ekran_2Satir);
    segment = Alt_Ekran_Baslangic;

    send_segment = (segment & 0x0F);
    Komut_Gon(send_segment);           // segment adreslerini gonder
    send_segment = (segment & 0xF0);
    send_segment >>= 4;
    send_segment |= 0x10;
    Komut_Gon(send_segment);

    text -= 13;

    while(1)
    {
        say++;
        j = *text;
        text++;
        if(j == 'H')
        {
            j = 15;
        }
        else if(j == 'A')
        {
            j = 16;
        }
        else if(j == 'T')
        {
            j = 17;
        }
        else if(j == 'B')
        {
            j = 18;
        }
        else if(j == 'Y')
        {
            j = 19;
        }
        else if(j == 'L')
        {
            j = 20;
        }
        else if(j == 'N')
        {
            j = 21;
        }
        else
        {
            j -= 0x2B;
        }

        for(i = 0; i < 10; i++)
            if(j < 22)
            {
                Veri_Gon(1, Rakam_Tablo_Big_Alt[j][i]);    // yazilacak verileri gonder
            }
            else
            {
                Veri_Gon(1, 0);
            }

        if(say > 12)
        {
            break;
        }
    }
}
//------------------------------------------------------------------------------
//  LCD'nin Display Görünümündeki Kýsmýna Rakam Yazdýran  Alt program
//  Altprograma giren argümanlar
//  rakam       = Displaya yazýlacak rakam
//  display     = Hangi displaya yazýlacagý bilgisi
//------------------------------------------------------------------------------
void Display_Yaz(uint8_t rakam, uint8_t display)
{
    uint8_t *adres_pointer, * rakam_pointer;
    uint8_t	i;
    adres_pointer = (uint8_t *)pointer_display[display]; // kacýncý displaya yazýlacagýný gösteren pointer
    rakam_pointer = (uint8_t *)pointer_rakam[rakam];    // hangi rakamýn yazýlacagýný gösteren pointer
    Komut_Gon(Ikon_Ekran);                                    // displaylarýn yer aldýgý page adresi seçer
    for(i = 0; i < 7; i++)
    {
        Ikon_Durum(*adres_pointer, *rakam_pointer);
        adres_pointer += 1;       // adres pointer arttýr
        rakam_pointer += 1;       // rakam pointer arttýr
    }
}
//------------------------------------------------------------------------------
//  LCD'nin Display Görünümündeki String Halinde rakamlari Yazdýran Alt program
//  Altprograma giren argümanlar
//  ptr       = Displaya yazýlacak rakam stringi
//------------------------------------------------------------------------------
void Display_String_Yaz(char *ptr)
{
    uint8_t i, j;

    for(j = 0; j < 6; j++)
    {
        for(i = 0; i < 7; i++)
        {
            Ikon_Durum(pointer_display[j][i], pointer_rakam[*ptr - 48][i]);
        }
        ptr++;
    }
}



