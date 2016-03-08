#include <iostream>

//these include emulates arduino functions
#include "WindowsInclude.h"

//include arduino program
#include "break.h"
#include "keyboard.h"
#include "display.h"







void DoEvents()
{
    MSG msg;
    BOOL result;

    while ( ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
        result = ::GetMessage(&msg, NULL, 0, 0);
        if (result == 0) // WM_QUIT
        {
            ::PostQuitMessage(msg.wParam);
            break;
        }
        else if (result == -1)
        {
            // Handle errors/exit application, etc.
        }
        else
        {
            ::TranslateMessage(&msg);
            :: DispatchMessage(&msg);
        }
    }
}



/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv)
{
    unsigned long displayTime = 0;
    /*
    for(;;){
            int arrow;
            arrow = kbhit();
    if(arrow==0){//display[0][0]=0;

    }
    if(arrow!=0){

            char a=getch();
    printf("%c\n",a);
            display[0][0]=a;}

    }
    */






    setup();
    for(;;)
    {
//simulare tasti collegati ai bit
        WORD scan=windowsReadKeyboard();
        windowsInputBitSet(left_key  , TRUE);
        windowsInputBitSet(up_key , TRUE);
        windowsInputBitSet(right_key , TRUE);
        windowsInputBitSet(down_key , TRUE);
windowsInputBitSet(enter_key , TRUE);


        switch(scan)
        {
        case 37:
            windowsInputBitSet(left_key  , FALSE);
            break;
        case 38:
            windowsInputBitSet(up_key , FALSE);
            break;
        case 39:
            windowsInputBitSet(right_key , FALSE);
            break;
        case 40:
            windowsInputBitSet(down_key , FALSE);
            break;
 case 13:
            windowsInputBitSet(enter_key , FALSE);
            break;

        }




         if (millis() > displayTime)
        {
            displayTime=millis()+100;
            windowsShowDisplay();
        }


        loop();
        DoEvents();
    }

}

