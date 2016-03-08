#include <arduino.h>
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"
//#include "WindowsInclude.h"
#include "audio.h"
#include "runman.h"



void screenPlayNewStatus(char);

//1=force redraw next time screenmenu start
char cSPredraw=1;
//before leaving always set to 1 to reset next time you land here

#define SPlives 4


#define _sp_death 2
#define _sp_win 3
#define _sp_play 1
#define _sp_delay 0
char cSPstate=0;//0=wait for start,1=play,2=wait for fail,3=end of screen



int iSPpoints=0;//
int iSPscreen=0;//0,1,2... playfield
int iSPlevel=0;

xy xySPman;
//initial position of player. stored to restart the gameplay on death
xy xySPmanNow;
char cSPdirection=0;//0=stop

#define max_ghosts 5
xy xySPghosts[max_ghosts];
//initial position of the ghost. stored to restart the gameplay on death

#define max_stars 50
xy xySPstars[max_stars];
int iSPstars;
int iSPstarsFound;


//last position of the ghost
xy xySPghostsNow[max_ghosts];
int iSPghost;//how many???
int iSpghostSpeed;//iSMspeed;
unsigned long ulSPghostNextMove=0;
char cSPghostDirection[max_ghosts];
//ghost strategy: every time the ghost hit a wall
//it change direction. which one? with a 50% probability
//-casual
//-in the direction of the runner
//if the choosed  direction is closed then choose another one by chance
//...to be refined...



char cSPperiod=0;
unsigned long ulSPblink;
int iSPblinkingStar=0;//0...n
unsigned long ulSPdelay=0;
char cSPlives;

void screenPlayScrableStar()
{
    int i;
    for(i=0; i<iSPstars; i++)
    {
        int i2=random(iSPstars);
        xy xyTmp;
        xyTmp=xySPstars[i];
        xySPstars[i]=xySPstars[i2];
        xySPstars[i2]=xyTmp;
    }

}


//first redraw of the screen
void screenPlayRedraw(bool bReset)
{
    int i;
    displayClear();


    if(bReset==true)
    {
        displayPlayField(iSPscreen,&xySPman,
                         max_ghosts,xySPghosts,
                         max_stars,xySPstars);
    }
    else
    {

        displayPlayField(iSPscreen,&xySPman,
                         0,xySPghosts,
                         0,xySPstars);
        return;
    }




    iSPstarsFound=0;

    xySPmanNow=xySPman;

//how many ghosts?
    iSPghost=0;
    for(i=0; i<max_ghosts; i++)
    {
        xySPghostsNow[i]=xySPghosts[i];
        if(xySPghostsNow[i].x<99) iSPghost++;
    }

//how many stars?
    iSPstars=0;
    for(i=0; i<max_stars; i++)
    {
        if(xySPstars[i].x<99) iSPstars++;
    }

//sramble stars for blink effect
    screenPlayScrableStar();
}




void  screenPlayDrawStars(char op)
{
    int i;
    for(i=0; i<iSPstars; i++)
    {
        if(xySPstars[i].x<99)
        {
            displayPixel(xySPstars[i].x,xySPstars[i].y,op);
        }
    }
}

void    screenPlayDrawRunner(char op)
{
    displayPixel(xySPmanNow.x,xySPmanNow.y,op);
}


void screenPlayDrawStar(int star,char op)
{
    if(xySPstars[star].x<99)
    {
        displayPixel(xySPstars[star].x,xySPstars[star].y,op);

    }
}
void  screenPlayDrawGhosts(char op)
{
    int i;
    for(i=0; i<iSPghost; i++)
    {
        if(xySPghostsNow[i].x<99)
        {
            displayPixel(xySPghostsNow[i].x,xySPghostsNow[i].y,op);
        }
    }
}


//eat a star. could be the last one so skip to next screen
void screenPlayEat(int i)
{
    if(xySPstars[i].x<99)
    {

        if(cSMsound!=0&&songPlaying==0)
        {
            play_rtttl_stop();
            play_rtttl_start_no_repeat(soundPing);
        }


        //real star
        iSPpoints++;
        xySPstars[i].x=99;
        xySPstars[i].y=99;
        //is there at least another star left?
        iSPstarsFound++;
        if(iSPstarsFound==iSPstars)
        {
            screenPlayNewStatus(_sp_win);//end of screen
        }
    }
}


void screenPlayMoveRunner(int direction)
{
    int i;
    bool bMoveAnyway=false;
    xy xyNext=xySPmanNow;

    switch(direction)
    {
    case up_key:
        xyNext.y--;
        if(xyNext.y<0) xyNext.y=15;
        break;
    case down_key:
        xyNext.y++;
        if(xyNext.y>15) xyNext.y=0;
        break;
    case left_key:
        xyNext.x--;
        if(xyNext.x<0) xyNext.x=15;
        break;
    case right_key:
        xyNext.x++;
        if(xyNext.x>15) xyNext.x=0;
        break;
    }

//going over a ghost?
    for(i=0; i<iSPghost; i++)
    {
        if(xyNext.x==xySPghostsNow[i].x&&xyNext.y==xySPghostsNow[i].y)
        {
            //death!!!
            screenPlayNewStatus(_sp_death);//death
            bMoveAnyway=true;
            break;
        }
    }
//going over a star?
    for(i=0; i<iSPstars; i++)
    {
        if(xyNext.x==xySPstars[i].x&&xyNext.y==xySPstars[i].y)
        {
            screenPlayEat(i);
            //eath star!!!
            bMoveAnyway=true;
            break;
        }
    }

//going against a wall?
    if(bMoveAnyway==true||displayPixelState(xyNext.x,xyNext.y)==0)
    {
        screenPlayDrawRunner(_draw_reset);
        xySPmanNow=xyNext;
        screenPlayDrawRunner(_draw_set);
    }
}


void screenPlayMove(xy *xyNow, char cDirection)
{
    switch(cDirection)
    {
    case left_key:
        xyNow->x--;
        if(xyNow->x<0) xyNow->x=15;
        break;
    case right_key:
        xyNow->x++;
        if(xyNow->x>15) xyNow->x=0;
        break;
    case up_key:
        xyNow->y--;
        if(xyNow->y<0) xyNow->y=15;
        break;
    case down_key:
        xyNow->y++;
        if(xyNow->y>15) xyNow->y=0;
        break;
    }
}


//gost can move over runner, over stars, not over other ghost or against walls
char screenPlayCheckDirection(xy xyNow, char cDirection)
{
    int i;
    screenPlayMove( &xyNow,cDirection);

    //allow ghost over runner
    if(xyNow.x==xySPmanNow.x&&xyNow.y==xySPmanNow.y) return 0;

    //do not allow ghost over ghost
    for(i=0; i<iSPghost; i++)
    {
        if(xyNow.x==xySPghostsNow[i].x&&xyNow.y==xySPghostsNow[i].y) return 1;
    }

    //allow ghost over stars
    for(i=0; i<iSPstars; i++)
    {
        if(xyNow.x==xySPstars[i].x&&xyNow.y==xySPstars[i].y) return 0;
    }

    return displayPixelState(xyNow.x,xyNow.y);

}


void screenPlayMoveOneGhost(int iGhost)
{
//ghost move only in a free direction. no wall, no ghost. if it is locked
//do not move at all.
//let's check for allowed direction
    int i;
    int iFound;
    char cAllowedDirections[4]= {0,0,0,0};
    int iFoundDirections=0;
    xy xyNow=xySPghostsNow[iGhost];
    char cDirection;


    char cBackDirection=0;
    cDirection=cSPghostDirection[iGhost]; //LAST DIRECTION
    switch(cDirection)
    {
    case up_key:
        cBackDirection= down_key;
        break;
    case down_key:
        cBackDirection= up_key;
        break;
    case left_key:
        cBackDirection= right_key;
        break;
    case right_key:
        cBackDirection= left_key;
        break;
    }


    if(screenPlayCheckDirection(xyNow,up_key)==0)
    {
        cAllowedDirections[iFoundDirections]=up_key;
        iFoundDirections++;
    }
    if(screenPlayCheckDirection(xyNow,down_key)==0)
    {
        cAllowedDirections[iFoundDirections]=down_key;
        iFoundDirections++;
    }
    if(screenPlayCheckDirection(xyNow,left_key)==0)
    {
        cAllowedDirections[iFoundDirections]=left_key;
        iFoundDirections++;
    }
    if(screenPlayCheckDirection(xyNow,right_key)==0)
    {
        cAllowedDirections[iFoundDirections]=right_key;
        iFoundDirections++;
    }

//IF ALLOWED DIRECTION >=2 THEN REMOVE THE "GO BACK" ONE
    if(iFoundDirections>=2)
    {
        for(i=0; i<iFoundDirections; i++)
        {
            if(cAllowedDirections[i]==cBackDirection)
            {
                iFoundDirections--;
                if(i<3)
                {
                    memcpy(cAllowedDirections+i,cAllowedDirections+i+1,4-i-1);
                }
            }
        }
    }





    if(iFoundDirections>0)
    {
//7 out 10 times move toward the runner, otherwise by chance
        if(random(100)<70)
        {
            //one in the directrion of the runner
            cDirection=0;
            for(i=0; i<iFoundDirections; i++)
            {
                switch(cAllowedDirections[i])
                {
                case up_key:
                    if(xySPmanNow.y<xyNow.y) cDirection=up_key;
                    break;
                case down_key:
                    if(xySPmanNow.y>xyNow.y) cDirection=down_key;
                    break;
                case left_key:
                    if(xySPmanNow.x<xyNow.x) cDirection=left_key;
                    break;
                case right_key:
                    if(xySPmanNow.x>xyNow.x)     cDirection=right_key;
                    break;

                }

                if(cDirection!=0)
                {
                    break;
                }
            }

            if(cDirection==0)
            {
//nothing found. by chance then
                cDirection=cAllowedDirections[random(iFoundDirections)];
            }

        }
        else
        {
            //random one
            cDirection=cAllowedDirections[random(iFoundDirections)];
        }
    }
    else
    {
        //stop ghost
        cDirection=0;
    }


    if(cDirection!=0)
    {
//really move ghost

        for(iFound=-1,i=0; i<iSPstars; i++)
        {
            if(xySPstars[i].x==xySPghostsNow[iGhost].x&&
                    xySPstars[i].y==xySPghostsNow[iGhost].y)
            {

                iFound=i;
                break;
            }
        }
        if(iFound>=0)
        {
            //ghost is over a star. redraw star
            displayPixel(xySPghostsNow[iGhost].x,xySPghostsNow[iGhost].y,_draw_set);
        }
        else
        {
            //reset ghost
            displayPixel(xySPghostsNow[iGhost].x,xySPghostsNow[iGhost].y,_draw_reset);

        }
        screenPlayMove(&xySPghostsNow[iGhost],cDirection);

        if(xySPghostsNow[iGhost].x==xySPmanNow.x&&
                xySPghostsNow[iGhost].y==xySPmanNow.y)
        {
            //death!!!!
            screenPlayNewStatus(_sp_death);

        }

    }
//remember last direction to prevent "go back"
    cSPghostDirection[iGhost]=cDirection;
}

void screenPlayMoveGhost()
{
    int i;
    for(i=0; i<iSPghost; i++)
    {
        if(xySPghosts[i].x<99)
        {
            screenPlayMoveOneGhost(i);
        }
    }
}




void screenPlayNewStatus(char cNewStatus)
{
    unsigned long ulNow=millis();

    if(cNewStatus==cSPstate) return;


    cSPstate=cNewStatus;
    switch(cNewStatus)
    {
    case _sp_play://start play
//        if(cSMsound!=0&&iSMtone>0)
//        {
//            play_rtttl_stop();
//            play_rtttl_start(iSMtone);
//       }
        break;
    case _sp_win://end of screen
        //play happy sound
        //set pause
        if(cSMsound!=0)
        {
            play_rtttl_stop();
            play_rtttl_start_no_repeat(soundWin);

        }
        ulSPdelay=ulNow+2000;

        displayClear();
        displayShowIconXY(_icon_LEVEL_1,0,0,1);
        displayShowIconXY(_icon_LEVEL_2,8,0,1);
iSPlevel++;
if(iSPlevel>9)
{
    displayShowIconXY(_icon_0+iSPlevel%100,4,8,0);
    displayShowIconXY(_icon_0+iSPlevel%10,8,8,0);
}
    else{displayShowIconXY(_icon_0+iSPlevel,6,8,0);}


        displayCopyToLed();



        break;
    case _sp_death://death
        //play happy sound
        //set pause
        if(cSMsound!=0)
        {
            play_rtttl_stop();
            play_rtttl_start_no_repeat(soundLoose);
        }
        ulSPdelay=ulNow+2000;
        cSPlives--;
        displayClear();
        displayShowIconXY(_icon_LIVES_1,0,0,1);
        displayShowIconXY(_icon_LIVES_2,8,0,1);

        displayShowIconXY(_icon_0+cSPlives,6,8,0);

        displayCopyToLed();

        break;
    }
}




//play task
char screenPlay(void)
{
    int i;

    unsigned long ulNow=millis();

    if(cSPredraw==1)
    {
        //reset all!!!!
        screenPlayRedraw(true);
        screenPlayDrawStars(_draw_set);
        cSPredraw=0;
        cSPperiod=0;
        ulSPblink=0;
        cSPdirection=0;
        iSPblinkingStar=0;
        iSpghostSpeed=iSMspeed;
        ulSPdelay=0;
        cSPlives=3;
        iSPlevel=1;
        iSPscreen=0;
        cSPstate=_sp_delay;
        memset(cSPghostDirection,0,max_ghosts);
        ulSPghostNextMove=ulNow+3000;

        if(cSMsound!=0)
        {
            play_rtttl_stop();
            play_rtttl_start_no_repeat(0);
        }
    }


    if(ulNow>ulSPghostNextMove&&cSPstate<=_sp_play)
    {
        ulSPghostNextMove=ulNow+1500-100*iSpghostSpeed;
//time to move all the ghost
        screenPlayMoveGhost();
    }










//total blink period is 0,4seconds=400mills.
//player stay on 100ms and off 100 ms
//star stay on 100ms and off 300 ms
//ghost on 200ms and off 200 ms
//cSPperiod remember the present period
//cSPperiod=0-->player+ghost
//cSPperiod=1-->ghost
//cSPperiod=2-->player
//cSPperiod=3-->star
    if(ulSPblink<ulNow&&cSPstate<=_sp_play)
    {
        //stars are switched off sequentially. every time a star switch off and the
        //previous one is restored. check for "no stars anymore" situation

        //restore old one
        screenPlayDrawStar(iSPblinkingStar,_draw_set);
        //look for the next one
        iSPblinkingStar++;
        if(iSPblinkingStar>iSPstars)
        {
            iSPblinkingStar=0;
            screenPlayScrableStar();
        }
        screenPlayDrawStar(iSPblinkingStar,_draw_reset);


        ulSPblink=ulNow+100;
        switch(cSPperiod)
        {
        case 0:
            //screenDrawStars(_draw_reset);
            screenPlayDrawRunner(_draw_set);
            screenPlayDrawGhosts(_draw_set);
            break;
        case 1:
            screenPlayDrawRunner(_draw_reset);
            break;
        case 2:
            screenPlayDrawGhosts(_draw_reset);
            break;
        case 3:
            break;
        }
        displayCopyToLed();
        cSPperiod++;
        if(cSPperiod==4) cSPperiod=0;

    }



    if(ulSPdelay<ulNow)
    {
        if(cSPstate==_sp_win)
        {
            //restart after win

            //increase monster speed
            iSpghostSpeed++;
            if(iSpghostSpeed>9)
            {
                iSpghostSpeed = 9;
            }
//ship to next playfield
            iSPscreen++;
            if(iSPscreen==max_playfield)
            {
                iSPscreen=0;
            }
//reset screen
            screenPlayRedraw(true);
            screenPlayDrawStars(_draw_set);
            cSPstate=_sp_play;
            memset(cSPghostDirection,0,max_ghosts);
            ulSPghostNextMove=ulNow+3000;


        }

        if(cSPstate==_sp_death)
        {
            //restart after death

            if(cSPlives<=0)
            {
                cSPredraw=1;
                return _screenEnd;
            }

            screenPlayRedraw(false);


            //return ghost and runner at the begin
            screenPlayDrawGhosts(_draw_reset);
            screenPlayDrawRunner(_draw_reset);

            for(int i=0; i<max_ghosts; i++)
            {
                xySPghostsNow[i]=xySPghosts[i];
            }

            xySPmanNow=xySPman;
            cSPstate=_sp_play;
            screenPlayDrawGhosts(_draw_set);
            screenPlayDrawRunner(_draw_set);
            memset(cSPghostDirection,0,max_ghosts);
            ulSPghostNextMove=ulNow+3000;
        }




//no more delay so read keyboard

        i=keyboardGetKey();
        switch(i)
        {
        case enter_key:
            if(cSPstate==_sp_delay)
            {
                //start play
                screenPlayNewStatus(_sp_play);
            }
            break;
        case up_key:
        case down_key:
        case left_key:
        case right_key:
            if(cSPstate==0)
            {
                //start play
                screenPlayNewStatus(_sp_play);
            }
            if(cSPstate==_sp_play)
            {
                //move if possible. iS There an obstacle?
                screenPlayMoveRunner(i);

            }
            break;
        }

    }



    return _screenPlay;
}

