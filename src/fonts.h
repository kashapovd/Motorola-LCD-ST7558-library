#include <avr/pgmspace.h>

const unsigned char font[] PROGMEM =
{
   0x00, 0x00, 0x00, 0x00, 0x00 ,   // sp  32
   0x00, 0x00, 0x2f, 0x00, 0x00 ,   // !   33
   0x00, 0x07, 0x00, 0x07, 0x00 ,   // "   34
   0x14, 0x7f, 0x14, 0x7f, 0x14 ,   // #   35
   0x24, 0x2a, 0x7f, 0x2a, 0x12 ,   // $   36
   0xc4, 0xc8, 0x10, 0x26, 0x46 ,   // %   37
   0x36, 0x49, 0x55, 0x22, 0x50 ,   // &   38
   0x00, 0x05, 0x03, 0x00, 0x00 ,   // '   39
   0x00, 0x1c, 0x22, 0x41, 0x00 ,   // (   40
   0x00, 0x41, 0x22, 0x1c, 0x00 ,   // )   41
   0x14, 0x08, 0x3E, 0x08, 0x14 ,   // *   42
   0x08, 0x08, 0x3E, 0x08, 0x08 ,   // +   43
   0x00, 0x00, 0x50, 0x30, 0x00 ,   // ,   44
   0x10, 0x10, 0x10, 0x10, 0x10 ,   // -   45
   0x00, 0x60, 0x60, 0x00, 0x00 ,   // .   46
   0x20, 0x10, 0x08, 0x04, 0x02 ,   // /   47
   0x3E, 0x51, 0x49, 0x45, 0x3E ,   // 0   48
   0x00, 0x42, 0x7F, 0x40, 0x00 ,   // 1   49
   0x42, 0x61, 0x51, 0x49, 0x46 ,   // 2   50
   0x21, 0x41, 0x45, 0x4B, 0x31 ,   // 3   51
   0x18, 0x14, 0x12, 0x7F, 0x10 ,   // 4   52
   0x27, 0x45, 0x45, 0x45, 0x39 ,   // 5   53
   0x3C, 0x4A, 0x49, 0x49, 0x30 ,   // 6   54
   0x01, 0x71, 0x09, 0x05, 0x03 ,   // 7   55
   0x36, 0x49, 0x49, 0x49, 0x36 ,   // 8   56
   0x06, 0x49, 0x49, 0x29, 0x1E ,   // 9   57
   0x00, 0x36, 0x36, 0x00, 0x00 ,   // :   58
   0x00, 0x56, 0x36, 0x00, 0x00 ,   // ;   59
   0x08, 0x14, 0x22, 0x41, 0x00 ,   // <   60
   0x14, 0x14, 0x14, 0x14, 0x14 ,   // =   61
   0x00, 0x41, 0x22, 0x14, 0x08 ,   // >   62
   0x02, 0x01, 0x51, 0x09, 0x06 ,   // ?   63
   0x32, 0x49, 0x59, 0x51, 0x3E ,   // @   64
   0x7E, 0x11, 0x11, 0x11, 0x7E ,   // A   65
   0x7F, 0x49, 0x49, 0x49, 0x36 ,   // B   66
   0x3E, 0x41, 0x41, 0x41, 0x22 ,   // C   67
   0x7F, 0x41, 0x41, 0x22, 0x1C ,   // D   68
   0x7F, 0x49, 0x49, 0x49, 0x41 ,   // E   69
   0x7F, 0x09, 0x09, 0x09, 0x01 ,   // F   70
   0x3E, 0x41, 0x49, 0x49, 0x7A ,   // G   71
   0x7F, 0x08, 0x08, 0x08, 0x7F ,   // H   72
   0x00, 0x41, 0x7F, 0x41, 0x00 ,   // I   73
   0x20, 0x40, 0x41, 0x3F, 0x01 ,   // J   74
   0x7F, 0x08, 0x14, 0x22, 0x41 ,   // K   75
   0x7F, 0x40, 0x40, 0x40, 0x40 ,   // L   76
   0x7F, 0x02, 0x0C, 0x02, 0x7F ,   // M   77
   0x7F, 0x04, 0x08, 0x10, 0x7F ,   // N   78
   0x3E, 0x41, 0x41, 0x41, 0x3E ,   // O   79
   0x7F, 0x09, 0x09, 0x09, 0x06 ,   // P   80
   0x3E, 0x41, 0x51, 0x21, 0x5E ,   // Q   81
   0x7F, 0x09, 0x19, 0x29, 0x46 ,   // R   82
   0x46, 0x49, 0x49, 0x49, 0x31 ,   // S   83
   0x01, 0x01, 0x7F, 0x01, 0x01 ,   // T   84
   0x3F, 0x40, 0x40, 0x40, 0x3F ,   // U   85
   0x1F, 0x20, 0x40, 0x20, 0x1F ,   // V   86
   0x3F, 0x40, 0x38, 0x40, 0x3F ,   // W   87
   0x63, 0x14, 0x08, 0x14, 0x63 ,   // X   88
   0x07, 0x08, 0x70, 0x08, 0x07 ,   // Y   89
   0x61, 0x51, 0x49, 0x45, 0x43 ,   // Z   90
   0x00, 0x7F, 0x41, 0x41, 0x00 ,   // [   91
   0x55, 0x2A, 0x55, 0x2A, 0x55 ,   // 55  92
   0x00, 0x41, 0x41, 0x7F, 0x00 ,   // ]   93
   0x04, 0x02, 0x01, 0x02, 0x04 ,   // ^   94
   0x40, 0x40, 0x40, 0x40, 0x40 ,   // _   95
   0x00, 0x01, 0x02, 0x04, 0x00 ,   // '   96
   0x20, 0x54, 0x54, 0x54, 0x78 ,   // a   97
   0x7F, 0x48, 0x44, 0x44, 0x38 ,   // b   98
   0x38, 0x44, 0x44, 0x44, 0x20 ,   // c   99
   0x38, 0x44, 0x44, 0x48, 0x7F ,   // d   100
   0x38, 0x54, 0x54, 0x54, 0x18 ,   // e   101
   0x08, 0x7E, 0x09, 0x01, 0x02 ,   // f   102
   0x0C, 0x52, 0x52, 0x52, 0x3E ,   // g   103
   0x7F, 0x08, 0x04, 0x04, 0x78 ,   // h   104
   0x00, 0x44, 0x7D, 0x40, 0x00 ,   // i   105
   0x20, 0x40, 0x44, 0x3D, 0x00 ,   // j   106
   0x7F, 0x10, 0x28, 0x44, 0x00 ,   // k   107
   0x00, 0x41, 0x7F, 0x40, 0x00 ,   // l   108
   0x7C, 0x04, 0x18, 0x04, 0x78 ,   // m   109
   0x7C, 0x08, 0x04, 0x04, 0x78 ,   // n   110
   0x38, 0x44, 0x44, 0x44, 0x38 ,   // o   111
   0x7C, 0x14, 0x14, 0x14, 0x08 ,   // p   112
   0x08, 0x14, 0x14, 0x18, 0x7C ,   // q   113
   0x7C, 0x08, 0x04, 0x04, 0x08 ,   // r   114
   0x48, 0x54, 0x54, 0x54, 0x20 ,   // s   115
   0x04, 0x3F, 0x44, 0x40, 0x20 ,   // t   116
   0x3C, 0x40, 0x40, 0x20, 0x7C ,   // u   117
   0x1C, 0x20, 0x40, 0x20, 0x1C ,   // v   118
   0x3C, 0x40, 0x30, 0x40, 0x3C ,   // w   119
   0x44, 0x28, 0x10, 0x28, 0x44 ,   // x   120
   0x0C, 0x50, 0x50, 0x50, 0x3C ,   // y   121
   0x44, 0x64, 0x54, 0x4C, 0x44 ,   // z   122
   
   0x00, 0x08, 0x36, 0x41, 0x00 ,   //7B- {
   0x00, 0x00, 0x7f, 0x00, 0x00 ,   //7C- |
   0x00, 0x41, 0x36, 0x08, 0x00 ,   //7D- }
   0x04, 0x02, 0x04, 0x08, 0x04 ,   //7E- ~
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //7F- 
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //80- Ђ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //81- Ѓ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //82- ‚
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //83- ѓ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //84- „
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //85- …
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //86- †
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //87- ‡
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //88- €
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //89- ‰
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //8A- Љ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //8B- ‹
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //8C- Њ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //8D- Ќ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //8E- Ћ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //8F- Џ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //90- ђ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //91- ‘
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //92- ’
   0x00, 0x00, 0x00, 0x00, 0x00 ,   // 93- “
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //94- ”
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //95- •
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //96- –
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //97- —
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //98- �
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //99- ™
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //9A- љ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //9B- ›
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //9C- њ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //9D- ќ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //9E- ћ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //9F- џ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A0-  
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A1- Ў
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A2- ў
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A3- Ј
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A4- ¤
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A5- Ґ
   0x00, 0x00, 0x36, 0x00, 0x00 ,   //A6- ¦
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A7- §
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A8- Ё
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //A9- ©
   0x3E, 0x49, 0x49, 0x49, 0x22 ,   //AA- Є
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //AB- «
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //AC- ¬
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //AD- ­
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //AE- ®
   0x44, 0x45, 0x7C, 0x45, 0x44 ,   //AF- Ї
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B0- °
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B1- ±
   0x00, 0x41, 0x7F, 0x41, 0x00 ,   //B2- І
   0x00, 0x44, 0x7D, 0x40, 0x00 ,   //B3- і
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B4- ґ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B5- µ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B6- ¶
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B7- ·
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B8- ё
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //B9- №
   0x38, 0x54, 0x44, 0x28, 0x00 ,   //BA- є
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //BB- »
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //BC- ј
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //BD- Ѕ
   0x00, 0x00, 0x00, 0x00, 0x00 ,   //BE- ѕ
   0x4A, 0x48, 0x7A, 0x40, 0x40 ,   //BF- ї
   // Русские буквы //
   0x7E, 0x11, 0x11, 0x11, 0x7E ,   // A        /*192*/
   0x7F, 0x49, 0x49, 0x49, 0x31 ,   // Б
   0x7F, 0x49, 0x49, 0x49, 0x36 ,   // B
   0x7F, 0x01, 0x01, 0x01, 0x03 ,   // Г
   0x60, 0x3E, 0x21, 0x21, 0x7F ,   // Д
   0x7F, 0x49, 0x49, 0x49, 0x41 ,   // E
   0x63, 0x14, 0x7F, 0x14, 0x63 ,   // Ж
   0x22, 0x49, 0x49, 0x49, 0x36 ,   // З
   0x7F, 0x10, 0x08, 0x04, 0x7F ,   // И
   0x7F, 0x10, 0x09, 0x04, 0x7F ,   // И
   0x7F, 0x08, 0x14, 0x22, 0x41 ,   // K
   0x7C, 0x02, 0x01, 0x01, 0x7F ,   // Л
   0x7F, 0x02, 0x0C, 0x02, 0x7F ,   // M
   0x7F, 0x08, 0x08, 0x08, 0x7F ,   // H
   0x3E, 0x41, 0x41, 0x41, 0x3E ,   // O
   0x7F, 0x01, 0x01, 0x01, 0x7F ,   // П
   0x7F, 0x09, 0x09, 0x09, 0x06 ,   // P
   0x3E, 0x41, 0x41, 0x41, 0x22 ,   // C
   0x01, 0x01, 0x7F, 0x01, 0x01 ,   // T
   0x07, 0x48, 0x48, 0x48, 0x3F ,   // У
   0x1C, 0x22, 0x7F, 0x22, 0x1C ,   // Ф
   0x63, 0x14, 0x08, 0x14, 0x63 ,   // X
   0x3F, 0x20, 0x20, 0x3F, 0x60 ,   // Ц
   0x07, 0x08, 0x08, 0x08, 0x7F ,   // Ч
   0x7F, 0x40, 0x7F, 0x40, 0x7F ,   // Ш
   0x3F, 0x20, 0x3F, 0x20, 0x7F ,   // Щ
   0x01, 0x7F, 0x48, 0x48, 0x30 ,   // Ъ
   0x7F, 0x48, 0x48, 0x30, 0x7F ,   // Ы
   0x7F, 0x48, 0x48, 0x48, 0x30 ,   // Ь
   0x22, 0x49, 0x49, 0x49, 0x3E ,   // Э
   0x7F, 0x08, 0x3E, 0x41, 0x3E ,   // Ю
   0x46, 0x29, 0x19, 0x09, 0x7F ,   // Я
   0x20, 0x54, 0x54, 0x54, 0x78 ,   // a
   0x78, 0x54, 0x54, 0x54, 0x20 ,   // b
   0x7C, 0x54, 0x54, 0x54, 0x28 ,   // в
   0x7C, 0x04, 0x04, 0x04, 0x00 ,   // г
   0x60, 0x38, 0x24, 0x38, 0x60 ,   // д
   0x38, 0x54, 0x54, 0x54, 0x18 ,   // e
   0x6C, 0x10, 0x7C, 0x10, 0x6C ,   // ж
   0x28, 0x44, 0x54, 0x54, 0x28 ,   // з
   0x3C, 0x40, 0x40, 0x20, 0x7C ,   // и
   0x3C, 0x40, 0x42, 0x20, 0x7C ,   // й
   0x7C, 0x10, 0x10, 0x28, 0x44 ,   // к
   0x60, 0x10, 0x08, 0x04, 0x7C ,   // л
   0x7C, 0x08, 0x10, 0x08, 0x7C ,   // м
   0x7C, 0x10, 0x10, 0x10, 0x7C ,   // н
   0x38, 0x44, 0x44, 0x44, 0x38 ,   // o
   0x7C, 0x04, 0x04, 0x04, 0x7C ,   // п
   0x7C, 0x14, 0x14, 0x14, 0x08 ,   // p
   0x38, 0x44, 0x44, 0x44, 0x20 ,   // c
   0x04, 0x04, 0x7C, 0x04, 0x04 ,   // т
   0x0C, 0x50, 0x50, 0x50, 0x3C ,   // y
   0x18, 0x24, 0x7C, 0x24, 0x18 ,   // ф
   0x44, 0x28, 0x10, 0x28, 0x44 ,   // x
   0x3C, 0x20, 0x20, 0x3C, 0x60 ,   // ц
   0x0C, 0x10, 0x10, 0x10, 0x7C ,   // ч
   0x7C, 0x40, 0x7C, 0x40, 0x7C ,   // ш
   0x3C, 0x20, 0x3C, 0x20, 0x7C ,   // щ
   0x04, 0x7C, 0x48, 0x48, 0x30 ,   // ъ
   0x7C, 0x48, 0x30, 0x00, 0x7C ,   // ы
   0x7C, 0x48, 0x48, 0x48, 0x30 ,   // ь
   0x28, 0x44, 0x54, 0x54, 0x38 ,   // э
   0x7C, 0x10, 0x38, 0x44, 0x38 ,   // ю
   0x58, 0x24, 0x24, 0x24, 0x7C     // я
};