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
*               *
* ****          *

*/




//1=force redraw next time screenmenu start
char cSSredraw=1;
//before leaving always set to 1 to reset next time you land here

unsigned long ulSSnextMove=0;


int iSSscreen=0;//0,1,2...


//first redraw of the screen
void screenSlideshowRedraw()
{
    displayClear();
    displayShowIconXY(_icon_schema_1a+iSSscreen+iSSscreen,0, 0,0);
    displayShowIconXY(_icon_schema_1b+iSSscreen+iSSscreen,8, 0 ,0);
    displayLineDy(0,0,16,0);
    displayLineDy(15,0,16,0);
    displayLineDx(0,0,16,0);

    displayShowIcon(_icon_DEMO_1,1);
    displayShowIcon(_icon_DEMO_2,3);


}


char screenSlideshow(void)
{
    unsigned long ulNow=millis();

    if(cSSredraw==1)
    {
        //reset all!!!!
        cSSredraw=0;
        ulSSnextMove=0;
        if(cSMsound!=0)
        {
            play_rtttl_start(iSMtone);
        }
    }

    if(ulSSnextMove<ulNow)
    {
        screenSlideshowRedraw();
        ulSSnextMove=ulNow+1000;
        iSSscreen++;
        if( iSSscreen==_max_schema )  iSSscreen =0;
    }



    switch(keyboardGetKey())
    {
    case up_key:
    case down_key:
    case left_key:
    case right_key:
    case enter_key:



        cSSredraw=1;
        play_rtttl_stop();
        return _screenMenu;
        break;
    }




    return _screenSlideshow;
}

