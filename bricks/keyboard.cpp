
#include <Arduino.h>

#include "keyboard.h"
int last_key=0;//last pressed key
long last_key_time=0;//at what time
int last_key_confirmed=0;
char last_key_recorded=0;


//autorepeat management
char cKeyboardAutorepeat=1;//1=autorepeat enabled
int iKeyboardAutorepeatDelay=300;//delay for next autorepeat key
unsigned long lKeyboardAutorepeatTime;//time of the last autorepeated key

unsigned long last_real_key=0;
unsigned long last_simulated_key=0;

void keyboardSetup()
{
    pinMode(left_key,INPUT_PULLUP);
    pinMode(up_key,INPUT_PULLUP);
    pinMode(right_key,INPUT_PULLUP);
    pinMode(down_key,INPUT_PULLUP);
    pinMode(enter_key,INPUT_PULLUP);

}

//is a key pressed?
void keyboardCheck()
{
    long lNow=millis();

    if(last_real_key==0)
    {
        last_real_key=lNow;
    }


    int presentKeyPressed=0;
    if(digitalRead(left_key)==0) presentKeyPressed = left_key;
    if(digitalRead(right_key)==0) presentKeyPressed = right_key;
    if(digitalRead(up_key)==0) presentKeyPressed = up_key;
    if(digitalRead(down_key)==0) presentKeyPressed = down_key;
    if(digitalRead(enter_key)==0) presentKeyPressed = enter_key;

//nothing pressed. reset all
    if(presentKeyPressed==0)
    {

        last_key=0;//last pressed key
        last_key_time=0;//at what time
        last_key_confirmed=0;
        last_key_recorded=0;

        if(lNow-last_real_key>10000)
        {
//10 seconds with no key pressed. simulate a keypress
//but not more then one per second
            if(lNow-last_simulated_key>500)
            {
                last_simulated_key=lNow;
                //disable simulated key         last_key_confirmed=2+random(4);
//2..3..4..5=2+0...3
            }
        }
        return;
    }

//real key pressed
    last_real_key=lNow;




//nothing was pressed before
    if(last_key==0)
    {
        last_key=presentKeyPressed;
        last_key_time=lNow;
        return;
    }

    if(last_key==presentKeyPressed)
    {

        if(cKeyboardAutorepeat!=0)
        {
            //autorepeat enabled
            if((lNow-last_key_time)>10  //delay antibounch
                    &&last_key_recorded==1
                    &&(lNow-lKeyboardAutorepeatTime)>iKeyboardAutorepeatDelay)
            {
//key re-confirmed!!!
                lKeyboardAutorepeatTime=lNow;
                iKeyboardAutorepeatDelay=80;//autorepeat in 80 milliseconds
                last_key_recorded=1;
                last_key_confirmed=presentKeyPressed;
                return;
            }

        }

        if(lNow-last_key_time>10  //delay antibounch
                &&last_key_recorded==0)
        {
//key confirmed!!!
            last_key_recorded=1;
            last_key_confirmed=presentKeyPressed;
            iKeyboardAutorepeatDelay=300;//set delay for autorepeat
            lKeyboardAutorepeatTime=lNow;
            return;
        }
    }
}


int keyboardGetKey()
{
    int key=last_key_confirmed;
    last_key_confirmed=0;
    return key;
}

void keyboardAutorepeat(char cOn)
{
    cKeyboardAutorepeat = cOn;
}

