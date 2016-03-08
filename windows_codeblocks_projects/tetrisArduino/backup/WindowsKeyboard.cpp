#include <windows.h>
#include <iostream>
#include "display.h"


//scancode 37 <     38 ^    39 >    40 v

WORD windowsReadKayboard()
{



    static HANDLE h=NULL;
    DWORD mode;
    BOOL success;
    INPUT_RECORD buffer[10];
    static WORD last_scancode=0;
    static BOOL last_key_down=FALSE;

    if(h==NULL)
    {
        h=GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(h, &mode);
        mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        mode |= ENABLE_WINDOW_INPUT;
        SetConsoleMode(h, mode);
    }

    DWORD count=0;
     GetNumberOfConsoleInputEvents(h,&count);
   if(count==0)
       {
    return last_scancode;
    }
    success=ReadConsoleInput(h, buffer, 10, &count);
    if (!success)
    {
    return last_scancode;
    }
    for (size_t i=0; i<count; ++i)
    {
        switch (buffer[i].EventType)
        {
        case KEY_EVENT:
        {
            WORD keycode=buffer[i].Event.KeyEvent.wVirtualKeyCode;
            if (buffer[i].Event.KeyEvent.bKeyDown)
            {
                if(last_key_down==TRUE&&keycode==last_scancode)
                {
                    //key repeated
                }
                else
                {
                    //std::cout<<"Key down: "<<keycode<<std::endl;
                    last_scancode=keycode;
                    last_key_down=TRUE;
                }
            }
            else
            {
                last_scancode=0;//keycode;
                last_key_down=FALSE;
                // std::cout<<"Key up:"<<keycode<<std::endl;
            }
            break;
        }
        default:
            break;
        }
    }
return last_scancode;
}
