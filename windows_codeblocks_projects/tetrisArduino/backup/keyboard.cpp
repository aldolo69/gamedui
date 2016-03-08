
#include "WindowsInclude.h"
#include "keyboard.h"
int last_key=0;//last pressed key
long last_key_time=0;//at what time
int last_key_confirmed=0;
char last_key_recorded=0;

void keyboardSetup()
{
    /*   pinMode(left_key,INPUT);
       pinMode(up_key,INPUT);
       pinMode(right_key,INPUT);
       pinMode(down_key,INPUT);
    */
}

//is a key pressed?
void keyboardCheck()
{
    int presentKeyPressed=0;
    if(digitalRead(left_key)!=0) presentKeyPressed = left_key;
    if(digitalRead(right_key)!=0) presentKeyPressed = right_key;
    if(digitalRead(up_key)!=0) presentKeyPressed = up_key;
    if(digitalRead(down_key)!=0) presentKeyPressed = down_key;

//nothing pressed. reset all
    if(presentKeyPressed==0)
    {
        last_key=0;//last pressed key
        last_key_time=0;//at what time
        last_key_confirmed=0;
        last_key_recorded=0;
        return;
    }

//nothing was pressed before
    if(last_key==0)
    {
        last_key=presentKeyPressed;
        last_key_time=millis();
        return;
    }

    if(last_key==presentKeyPressed)
    {
        if(millis()-last_key_time>10&&last_key_recorded==0)
        {
//key confirmed!!!
            last_key_recorded=1;
            last_key_confirmed=presentKeyPressed;
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
