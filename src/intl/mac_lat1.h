/* ISO Latin1 to Mac ROM code page mapping
                                                   UNICODE
 New         Note     Old UNICODE                   NAME
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
====      ========== ==== ======= ======================================== */
CONST UCHAR latin1_to_mac_rom[256] = {
	0x00,						/*             00  0000    NULL                                     */
	0x01,						/*             01  0001    START OF HEADING                         */
	0x02,						/*             02  0002    START OF TEXT                            */
	0x03,						/*             03  0003    END OF TEXT                              */
	0x04,						/*             04  0004    END OF TRANSMISSION                      */
	0x05,						/*             05  0005    ENQUIRY                                  */
	0x06,						/*             06  0006    ACKNOWLEDGE                              */
	0x07,						/*             07  0007    BELL                                     */
	0x08,						/*             08  0008    BACKSPACE                                */
	0x09,						/*             09  0009    HORIZONTAL TABULATION                    */
	0x0A,						/*             0A  000A    LINE FEED                                */
	0x0B,						/*             0B  000B    VERTICAL TABULATION                      */
	0x0C,						/*             0C  000C    FORM FEED                                */
	0x0D,						/*             0D  000D    CARRIAGE RETURN                          */
	0x0E,						/*             0E  000E    SHIFT OUT                                */
	0x0F,						/*             0F  000F    SHIFT IN                                 */
	0x10,						/*             10  0010    DATA LINK ESCAPE                         */
	0x11,						/*             11  0011    DEVICE CONTROL ONE                       */
	0x12,						/*             12  0012    DEVICE CONTROL TWO                       */
	0x13,						/*             13  0013    DEVICE CONTROL THREE                     */
	0x14,						/*             14  0014    DEVICE CONTROL FOUR                      */
	0x15,						/*             15  0015    NEGATIVE ACKNOWLEDGE                     */
	0x16,						/*             16  0016    SYNCHRONOUS IDLE                         */
	0x17,						/*             17  0017    END OF TRANSMISSION BLOCK                */
	0x18,						/*             18  0018    CANCEL                                   */
	0x19,						/*             19  0019    END OF MEDIUM                            */
	0x1A,						/*             1A  001A    SUBSTITUTE                               */
	0x1B,						/*             1B  001B    ESCAPE                                   */
	0x1C,						/*             1C  001C    FILE SEPARATOR                           */
	0x1D,						/*             1D  001D    GROUP SEPARATOR                          */
	0x1E,						/*             1E  001E    RECORD SEPARATOR                         */
	0x1F,						/*             1F  001F    UNIT SEPARATOR                           */
	0x20,						/*             20  0020    SPACE                                    */
	0x21,						/*             21  0021    EXCLAMATION MARK                         */
	0x22,						/*             22  0022    QUOTATION MARK                           */
	0x23,						/*             23  0023    NUMBER SIGN                              */
	0x24,						/*             24  0024    DOLLAR SIGN                              */
	0x25,						/*             25  0025    PERCENT SIGN                             */
	0x26,						/*             26  0026    AMPERSAND                                */
	0x27,						/*             27  0027    APOSTROPHE-QUOTE                         */
	0x28,						/*             28  0028    OPENING PARENTHESIS                      */
	0x29,						/*             29  0029    CLOSING PARENTHESIS                      */
	0x2A,						/*             2A  002A    ASTERISK                                 */
	0x2B,						/*             2B  002B    PLUS SIGN                                */
	0x2C,						/*             2C  002C    COMMA                                    */
	0x2D,						/*             2D  002D    HYPHEN-MINUS                             */
	0x2E,						/*             2E  002E    PERIOD                                   */
	0x2F,						/*             2F  002F    SLASH                                    */
	0x30,						/*             30  0030    DIGIT ZERO                               */
	0x31,						/*             31  0031    DIGIT ONE                                */
	0x32,						/*             32  0032    DIGIT TWO                                */
	0x33,						/*             33  0033    DIGIT THREE                              */
	0x34,						/*             34  0034    DIGIT FOUR                               */
	0x35,						/*             35  0035    DIGIT FIVE                               */
	0x36,						/*             36  0036    DIGIT SIX                                */
	0x37,						/*             37  0037    DIGIT SEVEN                              */
	0x38,						/*             38  0038    DIGIT EIGHT                              */
	0x39,						/*             39  0039    DIGIT NINE                               */
	0x3A,						/*             3A  003A    COLON                                    */
	0x3B,						/*             3B  003B    SEMICOLON                                */
	0x3C,						/*             3C  003C    LESS-THAN SIGN                           */
	0x3D,						/*             3D  003D    EQUALS SIGN                              */
	0x3E,						/*             3E  003E    GREATER-THAN SIGN                        */
	0x3F,						/*             3F  003F    QUESTION MARK                            */
	0x40,						/*             40  0040    COMMERCIAL AT                            */
	0x41,						/*             41  0041    LATIN CAPITAL LETTER A                   */
	0x42,						/*             42  0042    LATIN CAPITAL LETTER B                   */
	0x43,						/*             43  0043    LATIN CAPITAL LETTER C                   */
	0x44,						/*             44  0044    LATIN CAPITAL LETTER D                   */
	0x45,						/*             45  0045    LATIN CAPITAL LETTER E                   */
	0x46,						/*             46  0046    LATIN CAPITAL LETTER F                   */
	0x47,						/*             47  0047    LATIN CAPITAL LETTER G                   */
	0x48,						/*             48  0048    LATIN CAPITAL LETTER H                   */
	0x49,						/*             49  0049    LATIN CAPITAL LETTER I                   */
	0x4A,						/*             4A  004A    LATIN CAPITAL LETTER J                   */
	0x4B,						/*             4B  004B    LATIN CAPITAL LETTER K                   */
	0x4C,						/*             4C  004C    LATIN CAPITAL LETTER L                   */
	0x4D,						/*             4D  004D    LATIN CAPITAL LETTER M                   */
	0x4E,						/*             4E  004E    LATIN CAPITAL LETTER N                   */
	0x4F,						/*             4F  004F    LATIN CAPITAL LETTER O                   */
	0x50,						/*             50  0050    LATIN CAPITAL LETTER P                   */
	0x51,						/*             51  0051    LATIN CAPITAL LETTER Q                   */
	0x52,						/*             52  0052    LATIN CAPITAL LETTER R                   */
	0x53,						/*             53  0053    LATIN CAPITAL LETTER S                   */
	0x54,						/*             54  0054    LATIN CAPITAL LETTER T                   */
	0x55,						/*             55  0055    LATIN CAPITAL LETTER U                   */
	0x56,						/*             56  0056    LATIN CAPITAL LETTER V                   */
	0x57,						/*             57  0057    LATIN CAPITAL LETTER W                   */
	0x58,						/*             58  0058    LATIN CAPITAL LETTER X                   */
	0x59,						/*             59  0059    LATIN CAPITAL LETTER Y                   */
	0x5A,						/*             5A  005A    LATIN CAPITAL LETTER Z                   */
	0x5B,						/*             5B  005B    OPENING SQUARE BRACKET                   */
	0x5C,						/*             5C  005C    BACKSLASH                                */
	0x5D,						/*             5D  005D    CLOSING SQUARE BRACKET                   */
	0x5E,						/*             5E  005E    SPACING CIRCUMFLEX                       */
	0x5F,						/*             5F  005F    SPACING UNDERSCORE                       */
	0x60,						/*             60  0060    SPACING GRAVE                            */
	0x61,						/*             61  0061    LATIN SMALL LETTER A                     */
	0x62,						/*             62  0062    LATIN SMALL LETTER B                     */
	0x63,						/*             63  0063    LATIN SMALL LETTER C                     */
	0x64,						/*             64  0064    LATIN SMALL LETTER D                     */
	0x65,						/*             65  0065    LATIN SMALL LETTER E                     */
	0x66,						/*             66  0066    LATIN SMALL LETTER F                     */
	0x67,						/*             67  0067    LATIN SMALL LETTER G                     */
	0x68,						/*             68  0068    LATIN SMALL LETTER H                     */
	0x69,						/*             69  0069    LATIN SMALL LETTER I                     */
	0x6A,						/*             6A  006A    LATIN SMALL LETTER J                     */
	0x6B,						/*             6B  006B    LATIN SMALL LETTER K                     */
	0x6C,						/*             6C  006C    LATIN SMALL LETTER L                     */
	0x6D,						/*             6D  006D    LATIN SMALL LETTER M                     */
	0x6E,						/*             6E  006E    LATIN SMALL LETTER N                     */
	0x6F,						/*             6F  006F    LATIN SMALL LETTER O                     */
	0x70,						/*             70  0070    LATIN SMALL LETTER P                     */
	0x71,						/*             71  0071    LATIN SMALL LETTER Q                     */
	0x72,						/*             72  0072    LATIN SMALL LETTER R                     */
	0x73,						/*             73  0073    LATIN SMALL LETTER S                     */
	0x74,						/*             74  0074    LATIN SMALL LETTER T                     */
	0x75,						/*             75  0075    LATIN SMALL LETTER U                     */
	0x76,						/*             76  0076    LATIN SMALL LETTER V                     */
	0x77,						/*             77  0077    LATIN SMALL LETTER W                     */
	0x78,						/*             78  0078    LATIN SMALL LETTER X                     */
	0x79,						/*             79  0079    LATIN SMALL LETTER Y                     */
	0x7A,						/*             7A  007A    LATIN SMALL LETTER Z                     */
	0x7B,						/*             7B  007B    OPENING CURLY BRACKET                    */
	0x7C,						/*             7C  007C    VERTICAL BAR                             */
	0x7D,						/*             7D  007D    CLOSING CURLY BRACKET                    */
	0x7E,						/*             7E  007E    TILDE                                    */
	0x7F,						/*             7F  007F    DELETE                                   */
	0x20,						/* (REMAP)     80  0080    RESERVED - CTRL 80                       */
	0x20,						/* (REMAP)     81  0081    RESERVED - CTRL 81                       */
	0x20,						/* (REMAP)     82  0082    RESERVED - CTRL 82                       */
	0x20,						/* (REMAP)     83  0083    RESERVED - CTRL 83                       */
	0x20,						/* (REMAP)     84  0084    RESERVED - CTRL 84                       */
	0x20,						/* (REMAP)     85  0085    RESERVED - CTRL 85                       */
	0x20,						/* (REMAP)     86  0086    RESERVED - CTRL 86                       */
	0x20,						/* (REMAP)     87  0087    RESERVED - CTRL 87                       */
	0x20,						/* (REMAP)     88  0088    RESERVED - CTRL 88                       */
	0x20,						/* (REMAP)     89  0089    RESERVED - CTRL 89                       */
	0x20,						/* (REMAP)     8A  008A    RESERVED - CTRL 8A                       */
	0x20,						/* (REMAP)     8B  008B    RESERVED - CTRL 8B                       */
	0x20,						/* (REMAP)     8C  008C    RESERVED - CTRL 8C                       */
	0x20,						/* (REMAP)     8D  008D    RESERVED - CTRL 8D                       */
	0x20,						/* (REMAP)     8E  008E    RESERVED - CTRL 8E                       */
	0x20,						/* (REMAP)     8F  008F    RESERVED - CTRL 8F                       */
	0x20,						/* (REMAP)     90  0090    RESERVED - CTRL 90                       */
	0x20,						/* (REMAP)     91  0091    RESERVED - CTRL 91                       */
	0x20,						/* (REMAP)     92  0092    RESERVED - CTRL 92                       */
	0x20,						/* (REMAP)     93  0093    RESERVED - CTRL 93                       */
	0x20,						/* (REMAP)     94  0094    RESERVED - CTRL 94                       */
	0x20,						/* (REMAP)     95  0095    RESERVED - CTRL 95                       */
	0x20,						/* (REMAP)     96  0096    RESERVED - CTRL 96                       */
	0x20,						/* (REMAP)     97  0097    RESERVED - CTRL 97                       */
	0x20,						/* (REMAP)     98  0098    RESERVED - CTRL 98                       */
	0x20,						/* (REMAP)     99  0099    RESERVED - CTRL 99                       */
	0x20,						/* (REMAP)     9A  009A    RESERVED - CTRL 9A                       */
	0x20,						/* (REMAP)     9B  009B    RESERVED - CTRL 9B                       */
	0x20,						/* (REMAP)     9C  009C    RESERVED - CTRL 9C                       */
	0x20,						/* (REMAP)     9D  009D    RESERVED - CTRL 9D                       */
	0x20,						/* (REMAP)     9E  009E    RESERVED - CTRL 9E                       */
	0x20,						/* (REMAP)     9F  009F    RESERVED - CTRL 9F                       */
	0xCA,						/*             A0  00A0    NON-BREAKING SPACE                       */
	0xC1,						/*             A1  00A1    INVERTED EXCLAMATION MARK                */
	0xA2,						/*             A2  00A2    CENT SIGN                                */
	0xA3,						/*             A3  00A3    POUND SIGN                               */
	0xDB,						/*             A4  00A4    CURRENCY SIGN                            */
	0xB4,						/*             A5  00A5    YEN SIGN                                 */
	0x20,						/* (REMAP)     A6  00A6    BROKEN VERTICAL BAR                      */
	0xA4,						/*             A7  00A7    SECTION SIGN                             */
	0xAC,						/*             A8  00A8    SPACING DIAERESIS                        */
	0xA9,						/*             A9  00A9    COPYRIGHT SIGN                           */
	0xBB,						/*             AA  00AA    FEMININE ORDINAL INDICATOR               */
	0xC7,						/*             AB  00AB    LEFT POINTING GUILLEMET                  */
	0xC2,						/*             AC  00AC    NOT SIGN                                 */
	0x20,						/* (REMAP)     AD  00AD    SOFT HYPHEN                              */
	0xA8,						/*             AE  00AE    REGISTERED TRADE MARK SIGN               */
	0xF8,						/*             AF  00AF    SPACING MACRON                           */
	0xA1,						/*             B0  00B0    DEGREE SIGN                              */
	0xB1,						/*             B1  00B1    PLUS-OR-MINUS SIGN                       */
	0x20,						/* (REMAP)     B2  00B2    SUPERSCRIPT DIGIT TWO                    */
	0x20,						/* (REMAP)     B3  00B3    SUPERSCRIPT DIGIT THREE                  */
	0xAB,						/*             B4  00B4    SPACING ACUTE                            */
	0xB5,						/*             B5  00B5    MICRO SIGN                               */
	0xA6,						/*             B6  00B6    PARAGRAPH SIGN                           */
	0xE1,						/*             B7  00B7    MIDDLE DOT                               */
	0xFC,						/*             B8  00B8    SPACING CEDILLA                          */
	0x20,						/* (REMAP)     B9  00B9    SUPERSCRIPT DIGIT ONE                    */
	0xBC,						/*             BA  00BA    MASCULINE ORDINAL INDICATOR              */
	0xC8,						/*             BB  00BB    RIGHT POINTING GUILLEMET                 */
	0x20,						/* (REMAP)     BC  00BC    FRACTION ONE QUARTER                     */
	0x20,						/* (REMAP)     BD  00BD    FRACTION ONE HALF                        */
	0x20,						/* (REMAP)     BE  00BE    FRACTION THREE QUARTERS                  */
	0xC0,						/*             BF  00BF    INVERTED QUESTION MARK                   */
	0xCB,						/*             C0  00C0    LATIN CAPITAL LETTER A GRAVE             */
	0xE7,						/*             C1  00C1    LATIN CAPITAL LETTER A ACUTE             */
	0xE5,						/*             C2  00C2    LATIN CAPITAL LETTER A CIRCUMFLEX        */
	0xCC,						/*             C3  00C3    LATIN CAPITAL LETTER A TILDE             */
	0x80,						/*             C4  00C4    LATIN CAPITAL LETTER A DIAERESIS         */
	0x81,						/*             C5  00C5    LATIN CAPITAL LETTER A RING              */
	0xAE,						/*             C6  00C6    LATIN CAPITAL LETTER A E                 */
	0x82,						/*             C7  00C7    LATIN CAPITAL LETTER C CEDILLA           */
	0xE9,						/*             C8  00C8    LATIN CAPITAL LETTER E GRAVE             */
	0x83,						/*             C9  00C9    LATIN CAPITAL LETTER E ACUTE             */
	0xE6,						/*             CA  00CA    LATIN CAPITAL LETTER E CIRCUMFLEX        */
	0xE8,						/*             CB  00CB    LATIN CAPITAL LETTER E DIAERESIS         */
	0xED,						/*             CC  00CC    LATIN CAPITAL LETTER I GRAVE             */
	0xEA,						/*             CD  00CD    LATIN CAPITAL LETTER I ACUTE             */
	0xEB,						/*             CE  00CE    LATIN CAPITAL LETTER I CIRCUMFLEX        */
	0xEC,						/*             CF  00CF    LATIN CAPITAL LETTER I DIAERESIS         */
	0x20,						/* (REMAP)     D0  00D0    LATIN CAPITAL LETTER ETH                 */
	0x84,						/*             D1  00D1    LATIN CAPITAL LETTER N TILDE             */
	0xF1,						/*             D2  00D2    LATIN CAPITAL LETTER O GRAVE             */
	0xEE,						/*             D3  00D3    LATIN CAPITAL LETTER O ACUTE             */
	0xEF,						/*             D4  00D4    LATIN CAPITAL LETTER O CIRCUMFLEX        */
	0xCD,						/*             D5  00D5    LATIN CAPITAL LETTER O TILDE             */
	0x85,						/*             D6  00D6    LATIN CAPITAL LETTER O DIAERESIS         */
	0x20,						/* (REMAP)     D7  00D7    MULTIPLICATION SIGN                      */
	0xAF,						/*             D8  00D8    LATIN CAPITAL LETTER O SLASH             */
	0xF4,						/*             D9  00D9    LATIN CAPITAL LETTER U GRAVE             */
	0xF2,						/*             DA  00DA    LATIN CAPITAL LETTER U ACUTE             */
	0xF3,						/*             DB  00DB    LATIN CAPITAL LETTER U CIRCUMFLEX        */
	0x86,						/*             DC  00DC    LATIN CAPITAL LETTER U DIAERESIS         */
	0x20,						/* (REMAP)     DD  00DD    LATIN CAPITAL LETTER Y ACUTE             */
	0x20,						/* (REMAP)     DE  00DE    LATIN CAPITAL LETTER THORN               */
	0xA7,						/*             DF  00DF    LATIN SMALL LETTER SHARP S               */
	0x88,						/*             E0  00E0    LATIN SMALL LETTER A GRAVE               */
	0x87,						/*             E1  00E1    LATIN SMALL LETTER A ACUTE               */
	0x89,						/*             E2  00E2    LATIN SMALL LETTER A CIRCUMFLEX          */
	0x8B,						/*             E3  00E3    LATIN SMALL LETTER A TILDE               */
	0x8A,						/*             E4  00E4    LATIN SMALL LETTER A DIAERESIS           */
	0x8C,						/*             E5  00E5    LATIN SMALL LETTER A RING                */
	0xBE,						/*             E6  00E6    LATIN SMALL LETTER A E                   */
	0x8D,						/*             E7  00E7    LATIN SMALL LETTER C CEDILLA             */
	0x8F,						/*             E8  00E8    LATIN SMALL LETTER E GRAVE               */
	0x8E,						/*             E9  00E9    LATIN SMALL LETTER E ACUTE               */
	0x90,						/*             EA  00EA    LATIN SMALL LETTER E CIRCUMFLEX          */
	0x91,						/*             EB  00EB    LATIN SMALL LETTER E DIAERESIS           */
	0x93,						/*             EC  00EC    LATIN SMALL LETTER I GRAVE               */
	0x92,						/*             ED  00ED    LATIN SMALL LETTER I ACUTE               */
	0x94,						/*             EE  00EE    LATIN SMALL LETTER I CIRCUMFLEX          */
	0x95,						/*             EF  00EF    LATIN SMALL LETTER I DIAERESIS           */
	0x20,						/* (REMAP)     F0  00F0    LATIN SMALL LETTER ETH                   */
	0x96,						/*             F1  00F1    LATIN SMALL LETTER N TILDE               */
	0x98,						/*             F2  00F2    LATIN SMALL LETTER O GRAVE               */
	0x97,						/*             F3  00F3    LATIN SMALL LETTER O ACUTE               */
	0x99,						/*             F4  00F4    LATIN SMALL LETTER O CIRCUMFLEX          */
	0x9B,						/*             F5  00F5    LATIN SMALL LETTER O TILDE               */
	0x9A,						/*             F6  00F6    LATIN SMALL LETTER O DIAERESIS           */
	0xD6,						/*             F7  00F7    DIVISION SIGN                            */
	0xBF,						/*             F8  00F8    LATIN SMALL LETTER O SLASH               */
	0x9D,						/*             F9  00F9    LATIN SMALL LETTER U GRAVE               */
	0x9C,						/*             FA  00FA    LATIN SMALL LETTER U ACUTE               */
	0x9E,						/*             FB  00FB    LATIN SMALL LETTER U CIRCUMFLEX          */
	0x9F,						/*             FC  00FC    LATIN SMALL LETTER U DIAERESIS           */
	0x20,						/* (REMAP)     FD  00FD    LATIN SMALL LETTER Y ACUTE               */
	0x20,						/* (REMAP)     FE  00FE    LATIN SMALL LETTER THORN                 */
	0xD8						/*             FF  00FF    LATIN SMALL LETTER Y DIAERESIS           */
};
