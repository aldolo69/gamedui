//#include "arduino.h"
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"
#include "screenPlay.h"
#include "WindowsInclude.h"
#include "audio.h"
#include "runman.h"

//1=force redraw next time screen start
char cSEredraw=1;
unsigned long ulSEdelay;
char screenEnd(void)
{
    unsigned long ulNow=millis();

    if(cSEredraw==1)
    {
        cSEredraw=0;
        ulSEdelay=ulNow+2000;

        displayClear();
        displayShowIconXY(_icon_END_1,2,8,1);
        displayShowIconXY(_icon_END_2,10,8,1);

        displayShowIconXY(_icon_0+(iSPpoints%10000)/1000,0,0,0);
        displayShowIconXY(_icon_0+(iSPpoints%1000)/100, 4,0,0);
        displayShowIconXY(_icon_0+(iSPpoints%100)/10, 8,0,0);
        displayShowIconXY(_icon_0+(iSPpoints%10), 12,0,0);
    }

    if(ulSEdelay>ulNow)
    {
        return _screenEnd;
    }

    switch(keyboardGetKey())
    {
    case up_key:
    case down_key:
    case left_key:
    case right_key:
    case enter_key:
        cSEredraw=1;
        return _screenMenu;
        break;
    }




    return _screenEnd;
}
