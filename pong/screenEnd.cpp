#include <Arduino.h> 
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"
#include "screenPlay.h"
 
#include "audio.h"
#include "pong.h"


extern char cSPplayer;


//1=force redraw next time screen start
char cSEredraw=1;
unsigned long ulSEdelay;
unsigned long ulSEupdate;
char screenEnd(void)
{
    unsigned long ulNow=millis();

    if(cSEredraw==1)
    {
        cSEredraw=0;
        ulSEdelay=ulNow+2000;
        ulSEupdate=0;//ulNow+500;

        displayClear();

        displayShowIconXY(_icon_END_1,1,11,1);
        displayShowIconXY(_icon_END_2,9,11,1);

//        displayShowIconXY(_icon_0+(iSPpoints%10000)/1000,0,0,0);
        //      displayShowIconXY(_icon_0+(iSPpoints%1000)/100, 4,0,0);
        //    displayShowIconXY(_icon_0+(iSPpoints%100)/10, 8,0,0);
        //  displayShowIconXY(_icon_0+(iSPpoints%10), 12,0,0);
    }

    if(ulNow>ulSEupdate)
    {
        ulSEupdate=ulNow+500;
        if(cSPplayer==1)
        {
            displayShowIconXY(_icon_play1, 4,0,_draw_toggle);

        }
        else
        {
            displayShowIconXY(_icon_play2, 4,0,_draw_toggle);

        }

    }



    if(ulSEdelay>ulNow)//do not check the keyboard
    {
        return _screenEnd;
    }

    switch(keyboardGetKey())
    {
    case enter_key:
    case up_key:
    case down_key:
    case left_key:
    case right_key:
        cSEredraw=1;
        return _screenMenu;
        break;
    }




    return _screenEnd;
}
