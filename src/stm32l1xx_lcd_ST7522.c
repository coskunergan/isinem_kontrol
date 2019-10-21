/**
  ******************************************************************************
  * @file    stm32l1xx_lcd_ST7522.c
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
	*		- Bu k�t�panede CS1 pini kullanilmaz.
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
	*				Lcd_Str (Alt1,"Merhaba");		// glcd ye string g�nderir ve ikonlari d�zenler.
	*				Display_String_Yaz ("123456");		// Display b�l�m�ne string g�nderir.
	*				Ikon_Durum(Ikaz_Ikon,Yak);  / ekrandaki ikonlar1 yakar s�nd�r�r.
	*				Lcd_Clear(Alt_Ekran_2Satir,Alt_Ekran_Baslangic,Alt_Ekran_Bitis); // girilen adres degerlerinin
	*																																						arasindaki pixelleri siler.
  *  kisayol tr: isg��� ISG���
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

    if(GPIO_ReadOutputDataBit(Lcd_Enb_Port, Lcd_Enb_Pin)) // Lcd On konumunda degil ise Lcd yi a�
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
//  LCD'ye 1 Bytl�k Komut G�nderen Alt program
//  Altprograma giren arg�manlar
//  komut   = Lcd ye g�nderilecek komut. MSB --> LSB dogru bit bit g�nderilir
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
//  LCD'nin Alt Ekran�na veya �st Ekran�na 1 Bytl�k karakter G�nderen Alt program
//  Altprograma giren arg�manlar
//  ekrana    = Verilerin nas�l g�nderilecegini se�mek i�in kullan�l�r.Alttaki b�y�k ekrana
//  g�nderilecekse veriler LSB --> MSB ye dogru. �stteki k���k ekrana g�nderilecekse
//  veriler MSB --> LSB ye dogru g�nderilir.
//  ekran     = 1 ise alttaki b�y�k ekrana veri yaz�lacak demektir
//  ekran     = 0 ise �stteki k���k ekrana veri yaz�lacak demektir
//  veri      = Lcd ye g�nderilecek karakter
//------------------------------------------------------------------------------
void Veri_Gon(uint8_t ekran, uint8_t veri) // ekran altaki buyuk ekranam� yoksa ustteki kucuk
{
    // ekranam� yaz�lacag�n� secer.uste gonderirken byt bilgisi tersden gonderilmeli
    uint8_t i, bit;
    bit = 0x80;               // ekran = 0 ise veriyi MSB--> LSB ye dogru g�nder
    if(ekran == 1)
    {
        bit = 0x01;    // ekran = 1 ise veriyi LSB--> MSB ye dogru g�nder
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
//  LCD'deki Bir Ikonu Yakan/S�nd�ren Alt program
//  Altprograma giren arg�manlar
//  ikon_segment  = Lcd deki yak�lacak veya s�nd�r�lecek ikonun segment adresi
//  durum         =1 ise ikon yak�l�r , 0 ise s�nd�r�l�r.
//------------------------------------------------------------------------------
void Ikon_Durum(uint8_t ikon_segment, uint8_t durum)
{
    Komut_Gon(Ikon_Ekran);  // common olarak ikon page adersinin se�ilmesi
    Komut_Gon((ikon_segment & 0x0F));
    ikon_segment = (ikon_segment & 0xF0);
    ikon_segment >>= 4;
    ikon_segment |= 0x10;
    Komut_Gon(ikon_segment);
    Veri_Gon(0, durum);
}
//------------------------------------------------------------------------------
//  LCD'deki Display Cubuklar�ndan birini yakmak veya s�nd�rmek i�in
//  Tarihi_Lcd_Str altprogram�nda kullan�l�r
//  Altprograma giren arg�manlar
//  ikon_segment  = Lcd deki yak�lacak veya s�nd�r�lecek ikonun segment adresi
//  durum         =1 ise ikon yak�l�r , 0 ise s�nd�r�l�r.
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
//  LCD'nin Secilen K�sm�n� Temizleyen  Alt program
//  Altprograma giren arg�manlar
//  commons         = Lcd'nin hangi page adresinin temizlenecegini belirtir
//  segmnets        = Page adresinin kac�nc� segement adresinden baslan�lacag� bilgisi
//  segment_sinir   = segment adreslerinden nereye kadar devam edilecegi bilgisi
//------------------------------------------------------------------------------
void Lcd_Clear(uint8_t commons, uint8_t segments, uint8_t segments_sinir)
{
    uint8_t send_segment;
    // commons 0 'dan commons 7 'e kadar secilirse page adress=0xB0 b�y�k ekran birinci sat�r ve k���k ekran
    // commons 8 'dan commons 16 'e kadar secilirse Lcd_Clear=0xB1 ikinci satir
    // commons 17 'den sonras� Lcd_Clear=0xB8  ikonlar�n satir�
    // ust taraftaki kucuk satirin adresi 0 dan baslamal�

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
            if((segments == 16) && (commons == Ikon_Ekran)) // Ikonlar temizlenecekse adres 16dan sonra 47 kadar ikon olmad�g� i�in
            {
                segments = 47;    // aradaki adrs degerleri atlan�r
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
//  LCD'nin Ayarlar�n�n Yap�ld�g�  Alt program
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
        Komut_Gon(*ptr);      // Lcd initialize komutlar�n� g�nder
        ptr++;
    }
    while(*ptr != 0xFF);

    Lcd_Contrast(0x8);
    while(--t); // ~1mS
    Komut_Gon(0xAF);    // LCD On komutu

  //  GPIO_ResetBits(Lcd_BL_Port, Lcd_BL_Pin);        //  Backlight aktif

    Lcd_Clear(Alt_Ekran_1Satir, Ust_Ekran_Baslangic, Alt_Ekran_Bitis); // 0xB0 page adresini temizle (Ust Ekran + Alt Ekran 1. Sat�r)
    Lcd_Clear(Alt_Ekran_2Satir, Alt_Ekran_Baslangic, Alt_Ekran_Bitis); // 0xB1 page adresini temizle
    Lcd_Clear(Ikon_Ekran, Ikon_Baslangic, Ikon_Bitis);               // TL - �izgi -M3 ikonlar�n� temizle
}
//------------------------------------------------------------------------------
//  Tabloya Eklenen T�rk�e Karakterlerin G�r�nt�lenebilmesi ��in  Alt program
//  Altprograma giren arg�manlar
//  karakter    = yaz�lacak t�rk�e karakterin Ascii olarak gelen numaras�
//  Altprogramdan d�nen arg�manlar
//  cevrim      = yaz�lacak t�rk�e karakterin bizim tablomuzdaki s�ra numaras�
//------------------------------------------------------------------------------
char TurkceCevir(char karakter)
{
    char cevrim = 0;

    switch(karakter)
    {
        case 0xE7:
            cevrim = 0x7F;
            break;    // �
        case 0xF0:
            cevrim = 0x80;
            break;    // �
        case 0xFD:
            cevrim = 0x81;
            break;    // �
        case 0xF6:
            cevrim = 0x82;
            break;    // �
        case 0xFE:
            cevrim = 0x83;
            break;    // �
        case 0xFC:
            cevrim = 0x84;
            break;    // �
        case 0xC7:
            cevrim = 0x85;
            break;    // �
        case 0xD0:
            cevrim = 0x86;
            break;    // �
        case 0xDD:
            cevrim = 0x87;
            break;    // �
        case 0xD6:
            cevrim = 0x88;
            break;    // �
        case 0xDE:
            cevrim = 0x89;
            break;    // �
        case 0xDC:
            cevrim = 0x8A;
            break;    // �
        case 0xDF:
            cevrim = 0x8B;
            break;    // m3
        case 0xE0:
            cevrim = 0x8C;
            break;    // Su y�n� ters oku
        case 0xE1:
            cevrim = 0x8D;
            break;    // Su y�n� dogru oku
        case 0xE2:
            cevrim = 0x8E;
            break;    // Rf i�areti
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
//  LCD'nin Secilen K�sm�na G�nderilen Texti Yazan  Alt program
//  Altprograma giren arg�manlar
//  commons       = Lcd'nin hangi page adresine yaz�lacag�n� belirtir
//  start_Adr     = Page adresinin ka�inci karakter adresinden baslan�lacag� bilgisi
//  text          = Ekrana yaz�lacka olan tex bilgisi.
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
//  LCD'nin Secilen K�sm�na G�nderilen Texti Yazan  Alt program
//  Altprograma giren arg�manlar
//  commons       = Lcd'nin hangi page adresine yaz�lacag�n� belirtir
//  start_Adr     = Page adresinin ka�inci karakter adresinden baslan�lacag� bilgisi
//  text          = Ekrana yaz�lacka olan tex bilgisi.
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
//  LCD'nin Secilen K�sm�na G�nderilen Texti Yazan  Alt program
//  Altprograma giren arg�manlar
//  commons       = Lcd'nin hangi page adresine yaz�lacag�n� belirtir
//  start_Adr     = Page adresinin ka�inci karakter adresinden baslan�lacag� bilgisi
//  text          = Ekrana yaz�lacka olan tex bilgisi.
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
//  LCD'nin Display G�r�n�m�ndeki K�sm�na Rakam Yazd�ran  Alt program
//  Altprograma giren arg�manlar
//  rakam       = Displaya yaz�lacak rakam
//  display     = Hangi displaya yaz�lacag� bilgisi
//------------------------------------------------------------------------------
void Display_Yaz(uint8_t rakam, uint8_t display)
{
    uint8_t *adres_pointer, * rakam_pointer;
    uint8_t	i;
    adres_pointer = (uint8_t *)pointer_display[display]; // kac�nc� displaya yaz�lacag�n� g�steren pointer
    rakam_pointer = (uint8_t *)pointer_rakam[rakam];    // hangi rakam�n yaz�lacag�n� g�steren pointer
    Komut_Gon(Ikon_Ekran);                                    // displaylar�n yer ald�g� page adresi se�er
    for(i = 0; i < 7; i++)
    {
        Ikon_Durum(*adres_pointer, *rakam_pointer);
        adres_pointer += 1;       // adres pointer artt�r
        rakam_pointer += 1;       // rakam pointer artt�r
    }
}
//------------------------------------------------------------------------------
//  LCD'nin Display G�r�n�m�ndeki String Halinde rakamlari Yazd�ran Alt program
//  Altprograma giren arg�manlar
//  ptr       = Displaya yaz�lacak rakam stringi
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



