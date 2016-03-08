#include "keyboard.h"
#include "display.h"
#include "audio.h"
#include "WindowsInclude.h"
#include "arduino.h"
#include "screenMenu.h"
#include "screenPlay.h"
#include "screenEnd.h"

char screenActive=0;//0=setup,1=play,2=score



void setup(void)
{
    //Serial.begin(9600);
    //pinMode(13, OUTPUT);
    keyboardSetup();
}


void loop(void)
{


//check for pressed key
    keyboardCheck();


//loop among screens
    if(screenActive==_screenMenu) screenActive=screenMenu();
    if(screenActive==_screenPlay) screenActive=screenPlay();
    if(screenActive==_screenEnd) screenActive=screenEnd();



//play sound
    if (millis() > songTime)
    {
        if (play_rtttl() == 1)//end of sound???
        {
            songIndex = 0; //restart tone from the begin
            songTime=millis()+600;//pause before next tone
        }
    }



}


