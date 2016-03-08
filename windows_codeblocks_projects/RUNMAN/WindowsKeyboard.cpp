#include <windows.h>




//scancode 37 <     38 ^    39 >    40 v

HANDLE windowKeyboardH=NULL;
WORD last_scancode=0;
BOOL last_key_down=FALSE;

WORD windowsReadKeyboard()
{

    INPUT_RECORD windowKeyboardBuffer[10];


    DWORD mode;
    BOOL success;



    if(windowKeyboardH==NULL)
    {
        windowKeyboardH=GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(windowKeyboardH, &mode);
        mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        mode |= ENABLE_WINDOW_INPUT;
        SetConsoleMode(windowKeyboardH, mode);
    }

    DWORD count=0;
    GetNumberOfConsoleInputEvents(windowKeyboardH,&count);
    if(count==0)
    {
        return last_scancode;
    }
    success=ReadConsoleInput(windowKeyboardH, windowKeyboardBuffer, 10, &count);
    if (!success)
    {
        return last_scancode;
    }
    for (size_t i=0; i<count; ++i)
    {
        switch (windowKeyboardBuffer[i].EventType)
        {
        case KEY_EVENT:
        {
            WORD keycode=windowKeyboardBuffer[i].Event.KeyEvent.wVirtualKeyCode;
            if (windowKeyboardBuffer[i].Event.KeyEvent.bKeyDown)
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
