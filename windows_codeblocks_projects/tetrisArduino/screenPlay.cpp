#include "tetris.h"
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"
#include "WindowsInclude.h"
#include "audio.h"



/*

playfield 10X15
status area 4x16
*          *    -0
*          * ** |
*          * ** |
*          *    _3 next block
*          *     4
*          *  * -5
*          * ** |
*          *  * |
*          *  * |
*          * ***-9 level
*          *
*          *   *-11 fillup area. at the end add 1 to level
*          *   *|
*          *   *|
*          *   *|
****************-15
|____10____|_4_|
0          11
 1        10
            12 15
*/




//1=force redraw next time screenmenu start
char cSPredraw=1;
//before leaving always set to 1 to reset next time you land here


int iSPx=0;//block position
int iSPy=0;
int iSPblock=0;//present block
int iSPblockRotation=0;//0..3
int iSPblockNext=0;//next block
int iSPspeed=0;
unsigned long ulSPnextMove;
char cCollision=0;
int iSPpoints=0;
int iSPpointsBar=0;
int iSPbarLen=0;

//maybe rotation is possible with a little movement to left or right.
//let's test left and then right or right and then left according
//to this bias. every time you use the bias the sign change
int iSPbias=1;


//compute a random block between 0 and 5
char screenPlayRandomBlock()
{
    // return 5;
    return random(6);
}


//compute a new block
void screenPlayNewBlock()
{

    if(iSPblockNext>=0)//use next block
    {
        iSPblock=iSPblockNext;
        iSPblockNext=-1;//next block consumed
    }
    else//no next block so generate a new one
    {
        iSPblock=screenPlayRandomBlock();
    }
    if(iSPblockNext<0)//no next block? generate one
    {
        iSPblockNext=screenPlayRandomBlock();
    }
//add rotation to block
    iSPblockRotation=random(4);
    iSPx=5;
//let's ensure the block is right above the start of the playfield
//and not outside left or right boundaries
//iSPblok is the icon

    /*
    -**  0
    |**
    |
    |
    |
    |
    |
    -    7
    */

    unsigned char *p=iconCache;
    displayReadCache(_icon_tetris_1+iSPblock*4+iSPblockRotation);

    p+=7;

    for(int i=7; i>=0; i--,p--)
    {
        if(*p!=0)
        {
            iSPy=-i;
            break;
        }
    }

}



//move everything down 1 line
void screenPlayScroll(int y)
{
    for(int i=y-1; i>0; i--)
    {
        int i1=i+1;
        unsigned char byte1=display[(i>>3)][i&7];
        unsigned char byte2=display[2+(i>>3)][i&7];
        display[(i1>>3)][i1&7]=byte1;
        display[2+(i1>>3)][i1&7] = (display[2+(i1>>3)][i1&7]&0b00001111)
                                   | (byte2&0b11110000);
    }
    display[0][0]=0b10000000;
    display[2][0]=(display[2][0]&0b00001111) | 0b00010000;

}

//is a line full???
char screenPlayFull(int y)
{
    unsigned char byte1=display[(y>>3)][y&7];
    unsigned char byte2=display[2+(y>>3)][y&7];
    if(byte1==255&&(byte2&224)==224)
    {
        return 1;
    }
    return 0;
}

//remove completed line from the bottom

//*          *   *|
// 1111111=127 111=128+64+32=224

void screenPlayCleanup()
{
    for(int i=14; i>=0; i--)
    {
        while(screenPlayFull(i)!=0)
        {
            iSPpoints++;//another line completed
            iSPpointsBar++;
            screenPlayScroll(i);
        }
    }
}

//first redraw of the screen
void screenPlayRedraw()
{
    displayClear();
    displayLineDy(0,0,16,0);
    displayLineDy(11,0,16,0);
    displayLineDx(0,15,16,0);
    displayLineDy(15,11,6,0);

    displayShowIconXY(_icon_0+iSPspeed, 13,5,0);
    displayShowIconXY(_icon_tetris_1+iSPblockNext*4, 12,0,_draw_set);

}

//it is possible to move the block
//from iSPx,iSPy to iSPx+dx,iSPy+dy?
//before checking the new position it is
//necessary to remove the block from
//the current position
char screenPlayCheck(int dx , int dy,int rotation)
{
    char collision;
    int indexOld=_icon_tetris_1+iSPblock*4+iSPblockRotation;
    int indexNew=_icon_tetris_1+iSPblock*4+((iSPblockRotation+rotation)&3);
    displayShowIconXY(indexOld, iSPx,iSPy, _draw_reset);
    collision=displayShowIconXY(indexNew, iSPx+dx,iSPy+dy,_draw_collision);
    displayShowIconXY(indexOld, iSPx,iSPy, _draw_set);
    return collision;

}

char screenPlay(void)
{
    unsigned long ulNow=millis();

    if(cSPredraw==1)
    {
        //reset all!!!!
        randomSeed(ulNow);
        iSPspeed=iSMspeed;
        iSPblock=-1;
        iSPblockNext=-1;
        screenPlayNewBlock();//compute next block
        ulSPnextMove=0;
        cSPredraw=0;
        cCollision=0;
        iSPpoints=0;
        iSPpointsBar=0;
        iSPbarLen=0;
        screenPlayRedraw();

        if(cSMsound!=0)
        {
            play_rtttl_start(iSMtone);
        }
    }

    if(ulSPnextMove<ulNow)
    {
        ulSPnextMove=ulNow+100*(9-iSPspeed)+200;

        if(screenPlayCheck(0,1,0)==0)
        {
            cCollision=0;//no collision this time
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_reset);
            iSPy++;
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_set);
        }
        else
        {

//i'm at the bottom. let's look for full lines
            screenPlayCleanup();


            //end of game?
            //i'm not able to move down. maybe is time to choose another block
            if(cCollision==1)
            {
                //second time!! end of game
                play_rtttl_stop();
                cSPredraw=1;
                return _screenEnd;
            }
            else
            {
                //delete next block on screen
                displayShowIconXY(_icon_tetris_1+iSPblockNext*4, 12,0,_draw_reset);
                screenPlayNewBlock();//compute next block
                displayShowIconXY(_icon_tetris_1+iSPblockNext*4, 12,0,_draw_set);
                cCollision=1;
            }

        }

    }



    switch(keyboardGetKey())
    {
    case up_key:
    case enter_key:
        if(screenPlayCheck(0,0,1)==0)
        {
//rotation is possible
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_reset);
            iSPblockRotation=(iSPblockRotation+1)&3 ;
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_set);

        }
        else
        {
//try with a little movement
            if(screenPlayCheck(iSPbias,0,1)==0)
            {

                displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_reset);
                iSPx+=iSPbias;
                iSPblockRotation=(iSPblockRotation+1)&3 ;
                displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_set);
                iSPbias=-iSPbias;
            }
            else
            {
                if(screenPlayCheck(-iSPbias,0,1)==0)
                {

                    displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_reset);
                    iSPx+=-iSPbias;
                    iSPblockRotation=(iSPblockRotation+1)&3 ;
                    displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_set);
                    iSPbias=-iSPbias;
                }
            }
        }

        break;
    case down_key:
        ulSPnextMove=0;//force go-down
        break;
    case left_key:
        if(screenPlayCheck(-1,0,0)==0)
        {
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_reset);
            iSPx--;
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_set);
        }
        break;
    case right_key:
        if(screenPlayCheck(1,0,0)==0)
        {
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_reset);
            iSPx++;
            displayShowIconXY(_icon_tetris_1+iSPblock*4+iSPblockRotation, iSPx,iSPy, _draw_set);
        }
        break;
    }

//every 3 lines add a pixel to the bar
    if(iSPpointsBar>=1)
    {
        iSPpointsBar=0;
        iSPbarLen++;
        if(iSPbarLen==5)
        {
//new level!!!
//change level value if possible
            iSPbarLen=0;
            if(iSPspeed<9)
            {
                displayShowIconXY(_icon_0+iSPspeed, 13,5,_draw_reset);
                iSPspeed++;
                displayShowIconXY(_icon_0+iSPspeed, 13,5,0);
            }
//delete bar
            displayLineDy(13,11,4,_draw_reset);
            displayLineDy(14,11,4,_draw_reset);
        }
        else
        {
            displayLineDx(13,15-iSPbarLen,2,_draw_set);
        }
    }




    return _screenPlay;
}
