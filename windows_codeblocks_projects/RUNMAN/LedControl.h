#ifndef LEDCONTROL_H_INCLUDED
#define LEDCONTROL_H_INCLUDED

#include "WindowsDisplay.h"

class LedControl
{
public:
    LedControl(int i1,int i2,int i3, int i4)
    {
    }
    LedControl( )
    {
    }
    void shutdown(int i,int b) {}
    void setIntensity(int i,int x) {}
    void clearDisplay(int i) {}
    void setRow(int i,int row,unsigned char c)
    {
        displayW[i][row]=c;
    }


};

#endif // LEDCONTROL_H_INCLUDED
