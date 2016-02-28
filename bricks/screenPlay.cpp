#include <Arduino.h>
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"
#include "audio.h"
#include "bricks.h"

/*

playfield 14X15
***************** -0
*               *
*               *
*               *
*               *
*               *
*               *
*               *
*               *
*               *
*               *
*               *
*               *
*               *
* ****          *
* * * *         *
*/




//1=force redraw next time screenmenu start
char cSPredraw=1;
//before leaving always set to 1 to reset next time you land here

#define SPballs 4

int iSPspeed=0;
int iSPballs=3;
int iSPpadX=7;
int iSPpadDx=4;

char cSPstate=0;//0=wait for start,1=play,2=wait for fail


int iSPdx=0;//ball slope
int iSPdy=0;
int iSPx=0;//ball position
int iSPy=0;
unsigned long ulSPnextMove=0;//time of next ball move

int iSPpoints=0;//block smashed
int iSPscreen=0;//0,1,2... playfield


//is there any left pixel?
char screenPlayCheckEnd()
{
    for(int x=1; x<=14; x++)
    {
        for(int y=1; y<=7; y++)
        {
            if(x!=iSPx&&y!=iSPy)
            {
                if(displayPixelState(x,y)!=0) return 0;
            }
        }
    }
    return 1;//no pixel left to be removed
}


//first redraw of the screen
void screenPlayRedraw()
{
    displayClear();
    displayShowIconXY(_icon_schema_1a+iSPscreen+iSPscreen,0,0,0);
    displayShowIconXY(_icon_schema_1b+iSPscreen+iSPscreen,8,0,0);
    displayLineDy(0,0,16,0);
    displayLineDy(15,0,16,0);
    displayLineDx(0,0,16,0);
    displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
    displayPixel(iSPx,iSPy,_draw_set);
    for(int i=1; i<iSPballs; i++)
    {
        displayPixel(2+i+i,15,_draw_set);
    }

}

//do not redraw the background
void screenPlayRedrawBalls()
{
    int i=1;
    displayPixel(iSPx,iSPy,_draw_reset);

    displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
    for(; i<iSPballs; i++)
    {
        displayPixel(2+i+i,15,_draw_set);
    }
    for(; i<SPballs; i++)
    {
        displayPixel(2+i+i,15,_draw_reset);
    }

}





//return=1-->error! ball lost
//2=bounce against wall or pad
//0=no collision
//3=screen cleared!!!
char screenPlayBounce(int iX,int iY)
{
    char cRet=0;


    if(iX<1)//left wall
    {
        iSPdx=-iSPdx;//reflected
        cRet=2;
    }
    if(iX>14)//right wall
    {
        iSPdx=-iSPdx;//reflected
        cRet=2;
    }
    if(iY<1)//top wall
    {
        iSPdy=-iSPdy;//reflected
        cRet=2;
    }

    if(iY==14)//bottom?! is there the pad under it?
    {
        if(iSPpadX<=iX && (iSPpadX+iSPpadDx)>iX)
        {
            iSPdy=-iSPdy;//reflected
//if hit in the first half then increase slope
//otherwhise decrese it
            int iDxCorrection=(iX - iSPpadX - (iSPpadDx>>2) )*5;
            iSPdx+=iDxCorrection;
            cRet=2;
        }
    }

    if(iY>=15)//bottom?! is there the pad under it?
    {
        cRet=1;
    }


//is there a collision?
    if(cRet==0&&displayPixelState(iX,iY)!=0)
    {
//switch off pixel and bounch away
        displayPixel(iX,iY,_draw_reset);
        for(int i=1;; i++)
        {
            if(iX-i==0) break;
            if(iX-i==iSPx&&iY==iSPy) break;
            if(displayPixelState(iX-i,iY)==0) break;
            displayPixel(iX-i,iY,_draw_reset);
        }
        for(int i=1;; i++)
        {
            if(iX+i==15) break;
            if(iX+i==iSPx&&iY==iSPy) break;
            if(displayPixelState(iX+i,iY)==0) break;
            displayPixel(iX+i,iY,_draw_reset);
        }
        iSPpoints++;
        iSPdy=-iSPdy;//reflected

        if(screenPlayCheckEnd()!=0)
        {
            cRet=3;
        }
        else
        {
            cRet=2;
        }
    }

    return cRet;
}



/*
how to use slope: 8 octane to apply Bresenham's line algorithm

 2 1
3   0
4   7
 5 6

i need to move by 1 pixel, according to slope.
whenever slope change the algorithm start from fresh
if the slope stay constant then the algorithm goes on keeping the eps error running
*/

void screenPlayMoveStep(int x,int y,int dx,int dy,int *eps,int *iNextX,int *iNextY,int addX,int addY)
{
    *eps += dy;
    (*iNextX)+=addX;
    if ( ((*eps) << 1) >= dx )
    {
        (*iNextY)+=addY;
        *eps -= dx;
    }
}



//compute next position 1 pixel away according to slope
void screenPlayMove(int * piNextX,int * piNextY)
{
//slope according to iSPdx/iSPdy. which is the preset octane?
    char cSlope=0;
    if(iSPdx<0)
    {
        //2,3,4,5
        if(iSPdy<0)//iSPdx<0
        {
            //4,5
            if(iSPdx<iSPdy)
            {
                //4
                cSlope=4;
            }
            else
            {
                //5
                cSlope=5;
            }
        }
        else//iSpDy>=0 iSPdx<0
        {
            //2,3
            if((-iSPdx)>iSPdy)
            {
                //3
                cSlope=3;
            }
            else
            {
                //2
                cSlope=2;
            }
        }
    }
    else //iSPdx>=0
    {
        //0,1,7,6
        if(iSPdy<0)//iSPdx>=0
        {
            //6,7
            if((-iSPdy)>iSPdx)//iSPdx>=0
            {
                //6
                cSlope=6;
            }
            else
            {
                //7
                cSlope=7;
            }
        }
        else
        {
            //0,1
            if( iSPdy >iSPdx)//iSPdx>=0
            {
                //1
                cSlope=1;
            }
            else
            {
                //0
                cSlope=0;
            }
        }

    }


    /*
    for octane 0 it is possible to use:
    iSPx,iSPy=initial position
    iSPdx,iSPdy=delta
    */

    static int eps=0;
    static int oldDx=9999;
    static int oldDy=9999;

    if(oldDx!=iSPdx||oldDy!=iSPdy)
    {
        //initialize algorithm
        oldDx=iSPdx;
        oldDy=iSPdy;
        eps=0;
    }

    switch(cSlope)
    {
    case 0:
//        screenPlayMoveStep(x, y);
        screenPlayMoveStep(iSPx,iSPy,iSPdx,iSPdy,&eps,piNextX,piNextY,1,1);
        break;
    case 1:
//        screenPlayMoveStep(y, x);
        screenPlayMoveStep(iSPy,iSPx,iSPdy,iSPdx,&eps,piNextY,piNextX,1,1);
        break;
    case 2:
//        screenPlayMoveStep(y, -x);
        screenPlayMoveStep(iSPy,iSPx,iSPdy,-iSPdx,&eps,piNextY,piNextX,1,-1);
        break;
    case 3:
//        screenPlayMoveStep(-x, y);
        screenPlayMoveStep(iSPx,iSPy,-iSPdx,iSPdy,&eps,piNextX,piNextY,-1,1);
        break;
    case 4:
//        screenPlayMoveStep(-x, -y);
        screenPlayMoveStep(iSPx,iSPy,-iSPdx,-iSPdy,&eps,piNextX,piNextY,-1,-1);
        break;
    case 5:
//        screenPlayMoveStep(-y, -x);
        screenPlayMoveStep(iSPy,iSPx,-iSPdy,-iSPdx,&eps,piNextY,piNextX,-1,-1);
        break;
    case 6:
//        screenPlayMoveStep(-y, x);
        screenPlayMoveStep(iSPy,iSPx,-iSPdy,iSPdx,&eps,piNextY,piNextX,-1,1);
        break;
    case 7:
//        screenPlayMoveStep(x, -y);
        screenPlayMoveStep(iSPx,iSPy,iSPdx,-iSPdy,&eps,piNextX,piNextY,1,-1);
        break;
    }

}




//play task
char screenPlay(void)
{
    unsigned long ulNow=millis();

    if(cSPredraw==1)
    {
        //reset all!!!!
        iSPballs=SPballs;
        iSPspeed=iSMspeed;
        cSPredraw=0;
        iSPpoints=0;
        iSPpadX=7;
        iSPpadDx=4;
        iSPx=8;
        iSPy=13;
        iSPdx=0;
        iSPdy=100;
        cSPstate=0;
        screenPlayRedraw();
        ulSPnextMove=0;

    }

    if(ulSPnextMove<ulNow)
    {
        ulSPnextMove=ulNow+25*(9-iSPspeed)+50;
//move the ball
//iSPx,iSPy position
// iSPdx,iSPdy delta
        if(cSPstate==1)//play
        {
//which is the next ball position? add delta and check for:
//-wall bounce-->new position is not inside a wall so apply reflection

            int iNextX=iSPx;
            int iNextY=iSPy;
            screenPlayMove(&iNextX,&iNextY);
            switch(screenPlayBounce(iNextX,iNextY))
            {
            case 0://go straight
                displayPixel(iSPx ,iSPy ,_draw_reset);
                iSPx=iNextX;
                iSPy=iNextY;
                displayPixel(iSPx ,iSPy ,_draw_set);
                break;
            case 1://error
                cSPstate=0;//lost ball
                //lost ball
                iSPballs--;
                if( iSPballs>0)
                {//balls left so restart from here
                    screenPlayRedrawBalls();
                    iSPx=(iSPpadX+1);
                    iSPy=13;
                    displayPixel(iSPx ,iSPy ,_draw_set);

                }
                else
                {//no more balls
                    if(cSMsound!=0)
                    {
                        play_rtttl_stop();
                    }
                    cSPredraw=1;//next time redraw screen
                    return _screenEnd;//jump to end screen
                }
                break;
            case 2://bounce. do not move at all
                break;
            case 3://bounce against brick and end of screen
                cSPstate=0;//but no lost ball
                iSPscreen++;//switch to next playfield
                if(iSPscreen==_max_schema)
                {
                    if(iSPspeed<9) iSPspeed++;//increase speed
                    iSPscreen=0;//restart screens
                }
                iSPx=(iSPpadX+1);
                iSPy=13;
                screenPlayRedraw();
                    if(cSMsound!=0)
                    {
                        play_rtttl_stop();
                    }
            }
        }
    }



    switch(keyboardGetKey())
    {
    case up_key:
        if(cSPstate==0)
        {//start play
            cSPstate=1;
            iSPdx=(-7+iSPpadX)*10;
            iSPdy=-5*10;
            if(cSMsound!=0)
            {
                play_rtttl_start(iSMtone);
            }
        }
        break;
    case down_key:
        break;
    case left_key:
        if(iSPpadX>1)
        {//move pad
            displayLineDx(iSPpadX,14,iSPpadDx,_draw_reset);
            if(cSPstate==0)
            {//move ball with pad
                displayPixel(iSPx,iSPy,_draw_reset);
                iSPx--;
            }
            iSPpadX--;
            displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
            if(cSPstate==0)
            {//move ball with pad
                displayPixel(iSPx,iSPy,_draw_set);
            }
        }
        break;
    case right_key:

        if(iSPpadX+iSPpadDx<15)
        {
            displayLineDx(iSPpadX,14,iSPpadDx,_draw_reset);
            if(cSPstate==0)
            {//move ball with pad
                displayPixel(iSPx,iSPy,_draw_reset);
                iSPx++;
            }
            iSPpadX++;
            displayLineDx(iSPpadX,14,iSPpadDx,_draw_set);
            if(cSPstate==0)
            {//move ball with pad
                displayPixel(iSPx,iSPy,_draw_set);
            }
        }
        break;
    }

    return _screenPlay;
}
