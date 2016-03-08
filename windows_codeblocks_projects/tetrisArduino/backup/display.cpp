#include"mem.h"
#include "display.h"
unsigned char display[4][8];






unsigned char icons[][8]=
{
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
    0xE0, // ###
    0xA0, // # #
    0xA0, // # #
    0xA0, // # #
    0xE0, // ###
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


//tetris 1 a    16
    0b00000000,
    0b11110000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 1 b    17
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 1 c    18
    0b00000000,
    0b11110000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 1 d   19
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,


//tetris 2 a    20
    0b10000000,
    0b11110000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 2 b    21
    0b01100000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 2 c    22
    0b00000000,
    0b11110000,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 2 d   23
    0b01000000,
    0b01000000,
    0b01000000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,


//tetris 3 a    20
    0b00010000,
    0b11110000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 3 b    21
    0b01000000,
    0b01000000,
    0b01000000,
    0b01100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 3 c    22
    0b00000000,
    0b11110000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 3 d   23
    0b11000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,


//tetris 4 a    24
    0b11000000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 4 b    25
    0b11000000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 4 c    26
    0b11000000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 4 d   27
    0b11000000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

//tetris 5 a    28
    0b00110000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 5 b    29
    0b10000000,
    0b10000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 5 c    30
    0b00110000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 5 d   31
    0b10000000,
    0b10000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,



//tetris 6 a    32
    0b01000000,
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 6 b    33
    0b01000000,
    0b01100000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 6 c    34
    0b11100000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
//tetris 6 d   35
    0b01000000,
    0b11000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,




    0
};







void displayClear()
{
    memset(display,0,4*8);
}

//iconPosition=
//0 2
//1 3
void displayShowIcon(int iconIndex, int iconPosition)
{
    memcpy(display[iconPosition],icons[iconIndex],8);
}


/*le icone sono sempre 8*8 per semplicità. se una riga è 0
non si fa nulla. se è <> 0 si shifta di X e poi si cerca
dove inserire i 2 semibyte. se x=0 o x=8 non c'è nulla
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
    unsigned char *bytes=&icons[iconIndex][0];
    unsigned char byte;
    unsigned char byte1;
    unsigned char byte2;
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


