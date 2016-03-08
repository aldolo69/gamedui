#include "tetris.h"
#include "keyboard.h"
#include "display.h"
#include "screenMenu.h"
#include "screenPlay.h"
#include "WindowsInclude.h"
#include "audio.h"


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
        displayLineDy(12,0,16,_draw_reset);
        displayLineDy(13,0,16,_draw_reset);
        displayLineDy(14,0,16,_draw_reset);
        displayLineDy(15,0,16,_draw_reset);

        displayShowIconXY(_icon_0+(iSPpoints%1000)/100, 13,0,0);
        displayShowIconXY(_icon_0+(iSPpoints%100)/10, 13,5,0);
        displayShowIconXY(_icon_0+(iSPpoints%10), 13,10,0);
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
        cSEredraw=1;
        return _screenMenu;
        break;
    }




    return _screenEnd;
}
