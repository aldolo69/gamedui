#include <windows.h>
unsigned int digitalInput=0;


void windowsInputBitSet(int bit , boolean value)
{
    if(value==TRUE)
    {
        digitalInput|=1<<bit;
    }
    else
    {
        digitalInput&=255^1<<bit;
    }

}


void pinMode(int pin ,boolean direction)
{

}

int digitalRead(int pin)
{
    if(digitalInput&(1<<pin)) return 1 ;
    return 0;

}
