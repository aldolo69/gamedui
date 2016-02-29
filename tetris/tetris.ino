#include "keyboard.h"
#include "display.h"
#include "audio.h"
#include "tetris.h"
#include "screenMenu.h"
#include "screenPlay.h"
#include "screenEnd.h"

//which is the present displayed screen???
char screenActive = 0; //0=setup,1=play,2=score

//this is the next refresh time, every 50ms
unsigned long displayRefresh = 0;

void setup(void)
{
  //Serial.begin(38400);
  keyboardSetup();
  displayInitLed();
}


void loop(void)
{
  unsigned long ulNow = millis();

  if (ulNow > displayRefresh)
  {
    //time to refresh the screen?
    displayRefresh = ulNow + 50;
    displayCopyToLed();
  }

  //play sound
  if (ulNow > songTime)
  {
    //time to play next note?
    if (play_rtttl() == 1)//end of sound???
    {
      songIndex = 0; //restart tone from the begin
      songTime = ulNow + 1000; //pause before next tone
    }
  }

  //check for pressed key
  keyboardCheck();


  //loop among screens
  switch (screenActive)
  {
    case _screenMenu: screenActive = screenMenu();
      break;
    case _screenPlay: screenActive = screenPlay();
      break;
    case _screenEnd:  screenActive = screenEnd();
      break;
  }


}


