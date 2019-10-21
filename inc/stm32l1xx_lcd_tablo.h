// Lcd �zerindeki displaylerin dijitlerinin segment adresleri
unsigned char  const display1_8[7]={59,58,65,64,62,60,61}; 
unsigned char  const display2_8[7]={55,69,68,67,66,56,57};
unsigned char  const display3_8[7]={52,75,74,73,72,53,71};
unsigned char  const display4_8[7]={49,79,78,77,76,50,51};
unsigned char  const display5_8[7]={48,47,85,84,83,81,82};
unsigned char  const display6_8[7]={91,90,89,88,87,92,86};
unsigned char  const * const pointer_display[6]={display1_8,display2_8,display3_8,display4_8,display5_8,display6_8};
// Displaylere rakam yaz�labilmesi i�in yak�lmas� gereken display dijitleri 
unsigned char  const rakam0[7]={1,1,1,1,1,1,0};// 0
unsigned char  const rakam1[7]={0,1,1,0,0,0,0};// 1 // A B C D E F G
unsigned char  const rakam2[7]={1,1,0,1,1,0,1};// 2
unsigned char  const rakam3[7]={1,1,1,1,0,0,1};// 3
unsigned char  const rakam4[7]={0,1,1,0,0,1,1};// 4
unsigned char  const rakam5[7]={1,0,1,1,0,1,1};// 5
unsigned char  const rakam6[7]={1,0,1,1,1,1,1};// 6
unsigned char  const rakam7[7]={1,1,1,0,0,0,0};// 7
unsigned char  const rakam8[7]={1,1,1,1,1,1,1};// 8
unsigned char  const rakam9[7]={1,1,1,1,0,1,1};// 9
unsigned char  const bos[7]={0,0,0,0,0,0,0};// 10 bos
unsigned char  const * const pointer_rakam[11]={rakam0,rakam1,rakam2,rakam3,rakam4,rakam5,rakam6,rakam7,rakam8,rakam9,bos};
// Lcd initialize edilirken g�nderilen komutlar
unsigned char const Lcd_Komutlari[12]={0xF1,0x0F/*0x08*/,0xF8,0x01,0x06,0xF0,0x00,0x2D,0xA8,0xA3,0xFF};


// B�y�k karakter Rakam ve isaretler tablosu
unsigned char const Rakam_Tablo_Big_Ust[22][10] = {
													0x01,0x01,0x01,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,   // +
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   // , 
													0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,   // -	
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   // .
													0x70,0x50,0x70,0x03,0x0F,0x3C,0xF0,0x00,0x00,0x00,   // / => %
													0x7F,0xFF,0xE0,0xC0,0xC0,0xE0,0xFF,0x7F,0x00,0x00,   // 0	
													0x00,0x30,0x70,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,   // 1
													0x38,0x70,0xE0,0xC0,0xC0,0xE1,0x7F,0x3F,0x00,0x00,	 // 2
													0x30,0x60,0xC0,0xC1,0xC1,0x71,0x3F,0x1E,0x00,0x00,   // 3
													0x07,0x0F,0x1C,0x38,0x70,0xE0,0xC0,0x00,0x00,0x00,   // 4
													0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC1,0x00,0x00,   // 5
													0x7F,0xFF,0xC1,0xC1,0xC1,0xC1,0xC1,0x60,0x00,0x00,   // 6
													0xC0,0xC0,0xC0,0xC1,0xC3,0xC6,0xFC,0xF8,0x00,0x00,   // 7
													0x7C,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0x7C,0x00,0x00,   // 8
													0x7F,0xFF,0xC1,0xC1,0xC1,0xC1,0xFF,0x7F,0x00,0x00,   // 9
													0xFF,0xFF,0x01,0x01,0x01,0x01,0xFF,0xFF,0x00,0x00,   // H
													0x1F,0x3F,0x79,0xE1,0xE1,0x79,0x3F,0x1F,0x00,0x00,   // A
													0xC0,0xC0,0xC0,0xFF,0xFF,0xC0,0xC0,0xC0,0x00,0x00,	 // T
													0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0x7E,0x00,0x00,   // B
													0xF0,0xFC,0x0E,0x03,0x03,0x0E,0xFC,0xF0,0x00,0x00,   // Y
													0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   // L
													0xFF,0xFF,0x78,0x1F,0x03,0x00,0xFF,0xFF,0x00,0x00    // N
};

unsigned char const Rakam_Tablo_Big_Alt[22][10] = {
													0x80,0x80,0x80,0xF0,0xF0,0x80,0x80,0x80,0x00,0x00,   // +
													0x00,0x00,0x0D,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,   // , 
													0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,   // -		
													0x00,0x00,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00,   // .
													0x1E,0x78,0xE0,0x80,0x1C,0x14,0x1C,0x00,0x00,0x00,   // / => %
													0xFC,0xFE,0x0E,0x06,0x06,0x0E,0xFE,0xFC,0x00,0x00,   // 0
													0x00,0x06,0x06,0xFE,0xFE,0x06,0x06,0x00,0x00,0x00,   // 1	
													0x0E,0x1E,0x3E,0x76,0xE6,0xC6,0x86,0x06,0x00,0x00,   // 2
													0x18,0x0C,0x06,0x86,0x86,0x8C,0xFC,0x78,0x00,0x00,   // 3
													0xF8,0xF8,0x38,0x38,0xFE,0xFE,0x38,0x38,0x00,0x00,   // 4
													0x0C,0x06,0x06,0x06,0x06,0x06,0xFE,0xFC,0x00,0x00,   // 5
													0xFC,0xFE,0x86,0x86,0x86,0x86,0xFE,0xFC,0x00,0x00,   // 6
													0x00,0x00,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,	 // 7
													0xFC,0xFE,0x06,0x06,0x06,0x06,0xFE,0xFC,0x00,0x00,   // 8
												  0x0C,0x86,0x86,0x86,0x86,0x86,0xFE,0xFC,0x00,0x00,   // 9
													0xFE,0xFE,0x80,0x80,0x80,0x80,0xFE,0xFE,0x00,0x00,   // H
													0xFE,0xFE,0x80,0x80,0x80,0x80,0xFE,0xFE,0x00,0x00,   // A
 													0x00,0x00,0x00,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,   // T
													0xFE,0xFE,0x06,0x06,0x06,0x06,0xFE,0xFC,0x00,0x00,   // B
													0x00,0x00,0x00,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,   // Y
													0xFE,0xFE,0x06,0x06,0x06,0x06,0x0E,0x0E,0x00,0x00,   // L
													0xFE,0xFE,0x00,0x00,0xE0,0x78,0xFE,0xFE,0x00,0x00    // N
	
};


	



//ekrana yazdirilan harflerin kodlarini tutan tablo
unsigned char const Karakter_Tablo[111][6] = {                         
												 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // SPACE 
                         0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, // !
                         0x00, 0x03, 0x00, 0x03, 0x00, 0x00, // "
                         0x14, 0x3E, 0x14, 0x3E, 0x14, 0x00, // #
                         0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, // $
                         0x43, 0x33, 0x08, 0x66, 0x61, 0x00, // %
                         0x36, 0x49, 0x55, 0x22, 0x50, 0x00, // &
                         0x00, 0x05, 0x03, 0x00, 0x00, 0x00, // '
                         0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, // (
                         0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, // )
                         0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, // *
                         0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, // +
                         0x00, 0x50, 0x30, 0x00, 0x00, 0x00, // ,
                         0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // -
                         0x00, 0x60, 0x60, 0x00, 0x00, 0x00, // .
                         0x20, 0x10, 0x08, 0x04, 0x02, 0x00, // /
                         0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, // 0
                         0x04, 0x02, 0x7F, 0x00, 0x00, 0x00, // 1
                         0x42, 0x61, 0x51, 0x49, 0x46, 0x00, // 2
                         0x22, 0x41, 0x49, 0x49, 0x36, 0x00, // 3
                         0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, // 4
                         0x27, 0x45, 0x45, 0x45, 0x39, 0x00, // 5
                         0x3E, 0x49, 0x49, 0x49, 0x32, 0x00, // 6
                         0x01, 0x01, 0x71, 0x09, 0x07, 0x00, // 7
                         0x36, 0x49, 0x49, 0x49, 0x36, 0x00, // 8
                         0x26, 0x49, 0x49, 0x49, 0x3E, 0x00, // 9
                         0x00, 0x36, 0x36, 0x00, 0x00, 0x00, // :
                         0x00, 0x56, 0x36, 0x00, 0x00, 0x00, // ;
                         0x08, 0x14, 0x22, 0x41, 0x00, 0x00, // <
                         0x14, 0x14, 0x14, 0x14, 0x14, 0x00, // =
                         0x00, 0x41, 0x22, 0x14, 0x08, 0x00, // >
                         0x02, 0x01, 0x51, 0x09, 0x06, 0x00, // ?
                         0x00, 0x06, 0x09, 0x09, 0x06, 0x00, // @
                         0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00, // A
                         0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, // B
                         0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, // C
                         0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00, // D
                         0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, // E
                         0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, // F
                         0x3E, 0x41, 0x41, 0x49, 0x3A, 0x00, // G
                         0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, // H
                         0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, // I
                         0x30, 0x40, 0x40, 0x40, 0x3F, 0x00, // J
                         0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, // K
                         0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, // L
                         0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, // M
                         0x7F, 0x02, 0x04, 0x08, 0x7F, 0x00, // N
                         0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, // O
                         0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, // P
                         0x1E, 0x21, 0x21, 0x21, 0x5E, 0x00, // Q
                         0x7F, 0x09, 0x09, 0x09, 0x76, 0x00, // R
												 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, // S
                         0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, // T
                         0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, // U
                         0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, // V
                         0x7F, 0x20, 0x10, 0x20, 0x7F, 0x00, // W
                         0x63, 0x14, 0x08, 0x14, 0x63, 0x00, // X												 												 
                         0x07, 0x08, 0x70, 0x08, 0x07, 0x00, // Y
                         0x61, 0x51, 0x49, 0x45, 0x43, 0x00, // Z
                         0x00, 0x7F, 0x41, 0x00, 0x00, 0x00, // [
                         0x02, 0x04, 0x08, 0x10, 0x20, 0x00, // 
                         0x00, 0x00, 0x41, 0x7F, 0x00, 0x00, // ]
                         0x00, 0x06, 0x09, 0x09, 0x06, 0x00, // ^  -> derece simgesi �izili
                         0x40, 0x40, 0x40, 0x40, 0x40, 0x00, // _
                         0x00, 0x01, 0x02, 0x04, 0x00, 0x00, // `
                         0x20, 0x54, 0x54, 0x54, 0x78, 0x00, // a
                         0x7F, 0x44, 0x44, 0x44, 0x38, 0x00, // b
                         0x38, 0x44, 0x44, 0x44, 0x44, 0x00, // c
                         0x38, 0x44, 0x44, 0x44, 0x7F, 0x00, // d
                         0x38, 0x54, 0x54, 0x54, 0x18, 0x00, // e
                         0x04, 0x04, 0x7E, 0x05, 0x05, 0x00, // f
                         0x08, 0x54, 0x54, 0x54, 0x3C, 0x00, // g
                         0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, // h
                         0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, // i
                         0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, // j
                         0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, // k
                         0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, // l
                         0x7C, 0x04, 0x78, 0x04, 0x78, 0x00, // m
                         0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, // n
                         0x38, 0x44, 0x44, 0x44, 0x38, 0x00, // o
                         0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, // p
                         0x08, 0x14, 0x14, 0x14, 0x7C, 0x00, // q
                         0x00, 0x7C, 0x08, 0x04, 0x04, 0x00, // r
                         0x48, 0x54, 0x54, 0x54, 0x20, 0x00, // s
                         0x04, 0x04, 0x3F, 0x44, 0x44, 0x00, // t
                         0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, // u
                         0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, // v
                         0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, // w
                         0x44, 0x28, 0x10, 0x28, 0x44, 0x00, // x
                         0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, // y
                         0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, // z
                         0x00, 0x08, 0x36, 0x41, 0x41, 0x00, // {
                         0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, // |
                         0x41, 0x41, 0x36, 0x08, 0x00, 0x00, // }
                         0x02, 0x01, 0x02, 0x04, 0x02, 0x00, // ~
                         0x30, 0x48, 0xC8, 0x48, 0x00, 0x00, //�
                         0x10, 0xAA, 0xAA, 0xA8, 0x70, 0x00, //�
                         0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, //�
                         0x38, 0x45, 0x44, 0x45, 0x38, 0x00, //�
                         0x58, 0x54, 0xD4, 0x54, 0x30, 0x00, //�
                         0x38, 0x41, 0x40, 0x41, 0x38, 0x00, //�
                         0x3E, 0x41, 0xC1, 0x41, 0x22, 0x00, //�
                         0x3C, 0x43, 0x53, 0x53, 0x34, 0x00, //�
                         0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, //�
                         0x3C, 0x43, 0x42, 0x43, 0x3C, 0x00, //�
                         0x26, 0x49, 0xC9, 0x49, 0x32, 0x00, //�
                         0x3C, 0x41, 0x40, 0x41, 0x3C, 0x00, //�
                         0x00, 0x00, 0x11, 0x15, 0x0A, 0x00, //3
                         0x10, 0x38, 0x7C, 0xFE, 0x38, 0x38, // < (ok i�aretinin ba�lang�c�)
                         0x38, 0x38, 0xFE, 0x7C, 0x38, 0x10, // > (ok i�aretinin bitisi)
                         0x08, 0x24, 0x92, 0x24, 0x08, 0x00, // Rf i�areti   
                        };

unsigned char const Karakter_Tablo2[62][6] = {      // AK211 tablosu                    
                        0x3E, 0x48, 0x88, 0x48, 0x3E,0x00, //A 0
                        0xFE, 0x92, 0x92, 0x6C, 0x00,0x00, //B 1
                        0x7C, 0x82, 0x82, 0x82, 0x44,0x00, //C 2
                        0x78, 0x84, 0x86, 0x84, 0x48,0x00, //� 3
                        0xFE, 0x82, 0x82, 0x44, 0x38,0x00, //D 4
                        0xFE, 0x92, 0x92, 0x92, 0x92,0x00, //E 5
                        0xFE, 0x90, 0x90, 0x90, 0x80,0x00, //F 6
                        0x7C, 0x82, 0x92, 0x92, 0x5C,0x00, //G 7
                        0x1C, 0xA2, 0xAA, 0xAA, 0x0C,0x00, //G 8
                        0xFE, 0x10, 0x10, 0x10, 0xFE,0x00, //H 9
                        0x00, 0x82, 0xFE, 0x82, 0x00,0x00, //I 10
                        0x00, 0x22, 0xBE, 0x22, 0x00,0x00, //I 11
                        0x0C, 0x02, 0x82, 0xFC, 0x80,0x00, //J 12 
                        0xFE, 0x10, 0x28, 0x44, 0x82,0x00, //K 13
                        0x02, 0xFE, 0x82, 0x02, 0x02,0x00, //L 14
                        0xFE, 0x40, 0x30, 0x40, 0xFE,0x00, //M 15
                        0xFE, 0x60, 0x10, 0x0C, 0xFE,0x00, //N 16
                        0x7C, 0x82, 0x82, 0x82, 0x7C,0x00, //O 17
                        0x1C, 0xA2, 0x22, 0xA2, 0x1C,0x00, //� 18
                        0xFE, 0x90, 0x90, 0x60, 0x00,0x00, //P 19
                        0xFE, 0x90, 0x98, 0x94, 0x62,0x00, //R 20
                        0x64, 0x92, 0x92, 0x92, 0x4C,0x00, //S 21
                        0x64, 0x92, 0x96, 0x92, 0x4C,0x00, //S 22
                        0xC0, 0x82, 0xFE, 0x82, 0x80,0x00, //T 23
                        0xFC, 0x02, 0x02, 0x02, 0xFC,0x00, //U 24
                        0x3C, 0x82, 0x02, 0x82, 0x3C,0x00, //� 25
                        0xF0, 0x0C, 0x02, 0x0C, 0xF0,0x00, //V 26
                        0xC0, 0x20, 0x1E, 0x20, 0xC0,0x00, //Y 27
                        0x86, 0x8A, 0x92, 0xA2, 0xC2,0x00, //Z 28
                        0x00, 0x00, 0xFA, 0x00, 0x00,0x00, //! 29                        
                        0x7C, 0x8A, 0x92, 0xA2, 0x7C,0x00, //0 30
                        0x00, 0x42, 0xFE, 0x02, 0x00,0x00, //1 31
                        0x46, 0x8A, 0x92, 0x62, 0x00,0x00, //2 32 
                        0x00, 0x44, 0x92, 0x92, 0x6C,0x00, //3 33
                        0x18, 0x28, 0x4A, 0xFE, 0x0A,0x00, //4 34
                        0xE4, 0x92, 0x92, 0x92, 0x0C,0x00, //5 35
                        0x7C, 0x92, 0x92, 0x92, 0x0C,0x00, //6 36
                        0xC0, 0x8E, 0x90, 0xA0, 0xC0,0x00, //7 37
                        0x6C, 0x92, 0x92, 0x92, 0x6C,0x00, //8 38
                        0x64, 0x92, 0x92, 0x92, 0x7C,0x00, //9 39
                        0x00, 0x00, 0x00, 0x00, 0x00,0x00, // bosluk  40
                        0x00, 0x00, 0x48, 0x00, 0x00,0x00, // :       41
                        0x40, 0x8A, 0x90, 0x60, 0x00,0x00, // ?       42
                        0x00, 0x00, 0x0A, 0x0C, 0x00,0x00, // ,       43
                        0x02, 0x0C, 0x10, 0x60, 0x80,0x00, // /       44
                        0x00, 0x06, 0x06, 0x00, 0x00,0x00, // .       45
                        0x1E, 0x20, 0x3C, 0x20, 0x1E,0x00, //m        46                        
                        0xC6, 0x28, 0x10, 0x28, 0xC6,0x00, //X        47                        
                        0x78, 0x84, 0x94, 0x8C, 0x7A,0x00, //Q        48 
                        0xF8, 0x04, 0x1E, 0x04, 0xF8,0x00, //W        49
                        0x30, 0x30, 0x30, 0x30, 0x30,0x00, // ::::    50
                        0x64, 0x68, 0x30, 0x58, 0x98,0x00, // %       51
                        0x10, 0x38, 0x7C, 0xFE, 0x38,0x38, // < (ok isaretinin baslangici)   52
                        0x38, 0x38, 0x38, 0x38, 0x38,0x38, // :::::: (ok isaretinin g�vdesi) 53
                        0x38, 0x38, 0xFE, 0x7C, 0x38,0x10, // > (ok isaretinin bitisi)       54
                        0x00, 0x20, 0x48, 0x92, 0x48,0x20, // Rf isareti                     55
												0x10, 0x30, 0x7f, 0xff, 0x7f,0x30, // ok isareti yukari              56             
												0x10, 0x00, 0x00, 0x00, 0x00,0x00, // ok devam1	                     57
												0x00, 0xF0, 0x90, 0xF0, 0x00,0x00, // fransizca karakter             58->0x3A				
												0x5c, 0x92, 0x92, 0x92, 0x7c,0x00, // azerice karakter(ters e)       59->0x3B
												0x00, 0x02, 0x00, 0x00, 0x00,0x00, // tek pixel nokta olarak kull.   60->0x3C		
                        0xC0, 0x20, 0xFC, 0x20, 0xC0,0x00  // Rf isareti  16 byte            61->0x3D										
                        };
