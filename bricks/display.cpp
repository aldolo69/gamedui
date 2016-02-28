//#include"mem.h"
#include "display.h"
//#include"WindowsInclude.h"
#include "LedControl.h"



LedControl *displayLc;

unsigned char display[4][8];






const unsigned char icons[][8] PROGMEM={
//sound note 0
    0b00011000,
    0b00010110,
    0b00010000,
    0b00010000,
    0b01110000,
    0b11110000,
    0b01100000,
    0b00000000,
//no sound 1
    0b10011000,
    0b01010110,
    0b00110000,
    0b00010000,
    0b01111000,
    0b11110100,
    0b01100010,
    0b00000000,
//T 2
    0b11111000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,
//V 3
    0b10001000,
    0b10001000,
    0b10001000,
    0b01010000,
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,

//
//  Font data for Courier New 6pt
//
    // @0 '0' (3 pixels wide) //4
    0x40, //  #
    0xA0, // # #
    0xA0, // # #
    0xA0, // # #
    0x40, //  #
    0,0,0,
    // @5 '1' (3 pixels wide)
    0xC0, // ##
    0x40, //  #
    0x40, //  #
    0x40, //  #
    0xE0, // ###
    0,0,0,
    // @10 '2' (3 pixels wide)
    0xE0, // ###
    0x20, //   #
    0x40, //  #
    0x80, // #
    0xE0, // ###
    0,0,0,
    // @15 '3' (3 pixels wide)
    0xE0, // ###
    0x20, //   #
    0x40, //  #
    0x20, //   #
    0xE0, // ###
    0,0,0,
    // @20 '4' (3 pixels wide)
    0x20, //   #
    0x60, //  ##
    0xA0, // # #
    0xE0, // ###
    0x20, //   #
    0,0,0,
    // @25 '5' (3 pixels wide)
    0xE0, // ###
    0x80, // #
    0xC0, // ##
    0x20, //   #
    0xE0, // ###
    0,0,0,
    // @30 '6' (3 pixels wide)
    0x60, //  ##
    0x80, // #
    0xE0, // ###
    0xA0, // # #
    0xE0, // ###
    0,0,0,
    // @35 '7' (3 pixels wide)
    0xE0, // ###
    0x20, //   #
    0x40, //  #
    0x40, //  #
    0x40, //  #
    0,0,0,
    // @40 '8' (3 pixels wide)
    0xE0, // ###
    0xA0, // # #
    0x40, //  #
    0xA0, // # #
    0xE0, // ###
    0,0,0,
    // @45 '9' (3 pixels wide) //13
    0xE0, // ###
    0xA0, // # #
    0xE0, // ###
    0x20, //   #
    0xE0, // ###
    0,0,0,
//play //14
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b10000000,
    0b00000000,
//arrow//15
    0b00000000,
    0b00010000,
    0b00001000,
    0b11111100,
    0b00001000,
    0b00010000,
    0b00000000,
    0b00000000,

//slideshow 16
    0b11111110,
    0b11000010,
    0b10101010,
    0b10000110,
    0b11111110,
    0b00010000,
    0b01111100,
    0b00000000,
//END 17
    0b11110100,
    0b10000100,
    0b11000110,
    0b10000101,
    0b11110100,
    0b00000000,
    0b00000000,
    0b00000000,

    0b10111000,
    0b10100100,
    0b10100100,
    0b10100100,
    0b10111000,
    0b00000000,
    0b00000000,
    0b00000000,

//DEMO 19
    0b00000000,
    0b00000000,
    0b11001110,
    0b10101000,
    0b10101100,
    0b10101000,
    0b11001110,
    0b00000000,

    0b00000000,
    0b00000000,
    0b10010010,
    0b11110101,
    0b10010101,
    0b10010101,
    0b10010010,
    0b00000000,




//scheme 1a //16
    0b00000000,
    0b01110011,
    0b00000000,
    0b00111001,
    0b00000000,
    0b01110011,
    0b00000000,
    0b00000011,
 //scheme 1b //17
    0b00000000,
    0b10011100,
    0b00000000,
    0b11001110,
    0b00000000,
    0b10011100,
    0b00000000,
    0b10000000,

//scheme 2a //18
    0b00000000,
    0b00111000,
    0b00000001,
    0b00111000,
    0b00000001,
    0b00111000,
    0b00000001,
    0b00000000,
 //scheme 2b //19
    0b00000000,
    0b00011100,
    0b10000000,
    0b00011100,
    0b10000000,
    0b00011100,
    0b10000000,
    0b00000000,


//scheme 3a//19
    0b00000000,
    0b01110111,
    0b00000000,
    0b00111011,
    0b00000000,
    0b00001110,
    0b00000000,
    0b00000000,
 //scheme 3b //20
    0b00000000,
    0b01110000,
    0b00000000,
    0b10111000,
    0b00000000,
    0b00011100,
    0b00000000,
    0b00000000,


//scheme 4a//21
    0b00000000,
    0b00000001,
    0b00000011,
    0b00001100,
    0b00110000,
    0b00000110,
    0b00000001,
    0b00000110,
 //scheme 4b //22
    0b00000000,
    0b10000000,
    0b11000000,
    0b00110000,
    0b00001100,
    0b01100000,
    0b10000000,
    0b01100000,

//scheme 5a//23
    0b00000000,
    0b00001100,
    0b01100000,
    0b00001100,
    0b01100000,
    0b00001100,
    0b01100000,
    0b00001100,
 //scheme 5b //24
    0b00000000,
    0b00110000,
    0b00000110,
    0b00110000,
    0b00000110,
    0b00110000,
    0b00000110,
    0b00110000,

//scheme 6a//25
    0b00000000,
    0b11111111,
    0b00000000,
    0b11111111,
    0b00000000,
    0b11111111,
    0b00000000,
    0b11111111,
 //scheme 6b //26
    0b00000000,
    0b11111111,
    0b00000000,
    0b11111111,
    0b00000000,
    0b11111111,
    0b00000000,
    0b11111111,


//scheme 7a//27
    0b00000000,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b00000000,
 //scheme 7b //28
    0b00000000,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b00000000,


//scheme 8a//27
    0b00000000,
    0b00110011,
    0b00110000,
    0b00110011,
    0b00110000,
    0b00110011,
    0b00110000,
    0b00000011,
 //scheme 8b //28
    0b00000000,
    0b11001100,
    0b00001100,
    0b11001100,
    0b00001100,
    0b11001100,
    0b00001100,
    0b11000000,

    0
};



unsigned char iconCache[8];
int iconCacheIndex=-1;
void displayReadCache(int icon)
{
    if(icon==iconCacheIndex) return;

    iconCacheIndex=icon;
    memcpy_P((void *)iconCache, (void *)(icons[icon]), 8);

}


void displayClear()
{
    memset(display,0,4*8);
}

//iconPosition=
//0 2
//1 3
void displayShowIcon(int iconIndex, int iconPosition)
{
    displayReadCache(  iconIndex);
    memcpy(display[iconPosition],iconCache ,8);
}


/*le icone sono sempre 8*8 per semplicit�. se una riga � 0
non si fa nulla. se � <> 0 si shifta di X e poi si cerca
dove inserire i 2 semibyte. se x=0 o x=8 non c'� nulla
da shiftare
esempio: byte=10110101,x=6

byte1=00000010 byte>>6
byte2=11010100 byte<<8-6
operation=0-->or
operation=1-->xor per cancellare
operation=2=set
operation=4=check collision
*/
char displayShowIconXY(int iconIndex, int x,int y,char operation)
{
    int indice;
    unsigned char *bytes=iconCache;//&icons[iconIndex][0];
    unsigned char byte;
    unsigned char byte1=0;
    unsigned char byte2=0;

    displayReadCache(  iconIndex);

    int x2=8-x;

//out of screen????
    if(operation!=_draw_collision)
    {
        if(x>15||x<-7||y>15||y<-7) return 0;
    }

    for(indice=0; indice<8; indice++,y++)
    {
        byte=*bytes;//pixel da mostrare
        bytes++;
        if(byte==0&&operation!=2) continue;//non fare nulla

        if(operation!=_draw_collision)
        {
            if(y<0||y>15) continue;
        }

        switch(x)
        {
        case 0:
            byte1=byte;
            byte2=0;
            break;
        case 8:
            byte2=byte;
            byte1=0;
            break;
        default:
            if(x>0&&x<8)
            {
                byte1=byte>>x;
                byte2=byte<<(x2);
            }
            if(x>0&&x>8)
            {
                byte1=0;
                byte2=byte>>(x-8);
            }
            if(x<0)
            {
                byte1=byte<<(-x);
                byte2=0;
            }
        }
        /*adesso ho i 2 byte. devo trovare dove metterli in base a y
        */
//iconPosition=
//0 2
//1 3

        if(byte1!=0 )
        {
//va messo in 0 o 1
            if(operation==0)//or
            {
                display[y>>3][y&7]|=byte1;
            }
            if(operation==1)//xor
            {
                display[y>>3][y&7]^=byte1;
            }
            if(operation==2)//set
            {
                display[y>>3][y&7]=byte1;
            }
            if(operation==3)//reset
            {
                unsigned char *pByte=&display[y>>3][y&7];
                *pByte=*pByte&(255^byte1);
            }
            if(operation==4)//collision
            {
                if(y>=0)
                {
                    if(display[y>>3][y&7]&byte1) return 1;
                }
                else
                {
                    //we are checking against an out of screen icon.
                    //the playfield boundaries are extended
                    if(0b10000000&byte1) return 1;
                }
            }


        }
        if(byte2!=0 )
        {
//va messo in 0 o 1
            if(operation==0)//or
            {
                display[2+(y>>3)][y&7]|=byte2;
            }
            if(operation==1)//xor
            {
                display[2+(y>>3)][y&7]^=byte2;
            }
            if(operation==2)//set
            {
                display[2+(y>>3)][y&7]=byte2;
            }
            if(operation==3)//reset
            {
                unsigned char *pByte=&display[2+(y>>3)][y&7];
                *pByte=*pByte&(255^byte2);
            }
            if(operation==4)//collision
            {

                if(y>=0)
                {
                    if(display[2+(y>>3)][y&7]&byte2) return 1;
                }
                else
                {
                    //we are checking against an out of screen icon.
                    //the playfield boundaries are extended
                    if(0b00010000&byte2) return 1;
                }


            }
        }
    }
    return 0;
}





//set a single pixel
//0 2
//1 3
void displayPixel(int x,int y,char operation)
{
    if(x<0||x>15||y<0||y>15) return;

    int index=(x>7?2:0) + (y>7?1:0);
    unsigned char byte1=128>>(x&7);
    switch(operation)
    {
    case 0://or=set
        display[index][y&7]|=byte1;
        break;
    case 1://xor=toggle
        display[index][y&7]^=byte1;
        break;
    case 3://reset
        display[index][y&7]&=(255^byte1);
        break;
    }

}


unsigned char displayPixelState(int x,int y)
{
    if(x<0||x>15||y<0||y>15) return 0;

    int index=(x>7?2:0) + (y>7?1:0);
    unsigned char byte1=128>>(x&7);
   if((display[index][y&7]&byte1)!=0) return 1;
return 0;
}






void displayLineDx(int x,int y,int len,char operation)
{
    for(int i=0; i<len; i++)
    {
        displayPixel(x++,y,operation);
    }
}


void displayLineDy(int x,int y,int len,char operation)
{
    for(int i=0; i<len; i++)
    {
        displayPixel(x,y++,operation);
    }
}


/*
copy bytes from display memory to max chips. remember:

display[0]-->chip0 0000-->0000
display[1]-->chip2 0001-->0010
display[2]-->chip1 0010-->0001
display[3]-->chip3 0011-->0011

more over, byte are swapped top<->bottom
display[0][0]-->chip0[7]
display[0][1]-->chip0[6]
display[0][2]-->chip0[5]
display[0][3]-->chip0[4]
display[0][4]-->chip0[3]
display[0][5]-->chip0[2]
display[0][6]-->chip0[1]
display[0][7]-->chip0[0]
*/
//unfortunatly hardware bits are lsb to msb, the opposite of the display buffer!!! a bit mirror is required
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
void displayCopyToLed()
{
    int x1;
    for(int x=0; x<4; x++)
    {
        switch(x)
        {
        case 1:
            x1=2;
            break;
        case 2:
            x1=1;
            break;
        default:
            x1=x;
            break;
        }

        for(int y=0; y<8; y++)
        {
            displayLc->setRow(x1,7-y,  reverse(display[x][y])) ;
        }
    }
}




void displayInitLed()
{
    //LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
    displayLc=new LedControl(9,11,10,4);
    /*
     The MAX72XX is in power-saving mode on startup,
     we have to do a wakeup call
     */
    /* Set the brightness to a medium values */
    /* and clear the display */
    for(int i=0; i<4; i++)
    {
        displayLc->shutdown(i,false);
        displayLc->setIntensity(i,8);
        displayLc->clearDisplay(i);
    }
}

