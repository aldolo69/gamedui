#include "runman.h"
#include "keyboard.h"
#include "display.h"

/*
MENU:
>  t1/t2/t3 melodia
   Musica/NoMusica
   V1/V2/V3 velocit�
   Giocare

*/

//used to select menu item
int iSMtop=0;//0..2
int iSMarrow=0;//0 o 1


//setup values
char cSMsound=0;//1=play sound
int iSMspeed=1;//1..9 play speed



//1=force redraw next time screenmenu start
char cSMredraw=1;
//before leaving always set to 1 to reset next time you land here


void screenMenuRedrawSpeed(int iconindex)//2,3
{
    displayShowIconXY(_icon_v, 6,(iconindex-2)<<3,0);
    displayShowIconXY(_icon_0+iSMspeed, 12,(iconindex-2)<<3,0);
}
void screenMenuRedrawPlay(int iconindex)//2,3
{
    // displayShowIcon(_icon_play, iconindex);
    displayShowIconXY(_icon_play, 9,(iconindex-2)<<3,0);
}



void screenMenuRedrawSlideshow(int iconindex)//2,3
{
    // displayShowIcon(_icon_play, iconindex);
    displayShowIconXY(_icon_slideshow, 9,(iconindex-2)<<3,0);
}

void screenMenuRedrawSound(int iconindex)//2,3
{
    if(cSMsound!=0)
    {
        //    displayShowIcon(_icon_sound, iconindex);
        displayShowIconXY(_icon_sound, 7,(iconindex-2)<<3,0);
    }
    else
    {
        displayShowIconXY(_icon_no_sound, 7,(iconindex-2)<<3,0);
    }
}



//pressed -->. do action on menu
char screenMenuAction(int key)
{

//    >  t1/t2/t3 melodia
//   Musica/NoMusica
//   V1/V2/V3 velocit�
//   Giocare

    switch(iSMtop+iSMarrow)//0..3
    {
    case 0://sound
        cSMsound^=1;
        break;
    case 1://speed1..9
        if(key==right_key)
        {
            iSMspeed++;
            if(iSMspeed==10) iSMspeed =1;
        }
        if(key==left_key)
        {
            iSMspeed--;
            if(iSMspeed==0) iSMspeed =9;
        }
        break;
    case 2://play
        return _screenPlay;
//    case 3://play
//        return _screenSlideshow;

    }

    return _screenMenu;

}

void screenMenuRedraw()
{
    displayClear();
    displayShowIcon(_icon_arrow,  iSMarrow);
     if(iSMtop==0)
    {
        screenMenuRedrawSound(0+2);
        screenMenuRedrawSpeed(1+2);
    }
    if(iSMtop==1)
    {
        screenMenuRedrawSpeed(0+2);
        screenMenuRedrawPlay(1+2);
    }
  //  if(iSMtop==2)
  //  {
  //      screenMenuRedrawPlay(0+2);
  //      screenMenuRedrawSlideshow(1+2);
  //  }

}


char screenMenu()
{
    char cRedraw=0;

//globale per redraw
    if(cSMredraw==1)
    {
        cSMredraw=0;
        cRedraw=1;
    }


    int i=keyboardGetKey();

    if(i==up_key)
    {
        if(iSMarrow!=0)
        {
            iSMarrow--;
            cRedraw=1;
        }
        else
        {
            if(iSMtop>0)
            {
                iSMtop--;
                cRedraw=1;
            }
        }
    }
    if(i==down_key)
    {
        if(iSMarrow==0)
        {
            iSMarrow++;
            cRedraw=1;
        }
        else
        {
            if(iSMtop<1)
            {
                iSMtop++;
                cRedraw=1;
            }
        }
    }



    if(i==left_key||i==right_key||i==enter_key)
    {
        cSMredraw=1;//maybe we are leaving
        cRedraw=0;//aways redraw
        return screenMenuAction(i);
    }



    if(cRedraw!=0)
    {
        screenMenuRedraw();
    }

    return _screenMenu;
}
