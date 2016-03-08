
#include <iostream>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <stdio.h>     /* for printf() function */

HMIDIOUT midiplayerhMidi;
int midiplayerEnabled=0;
int midiplayerLastNoteOff=0;

double midiplayerRound (double n, int d)
{
    return (floor((n)*pow(10.0,(d))+0.5)/pow(10.0,(d)));
}







void noTone(int pin)
{
    if(midiplayerLastNoteOff==0) return;



    midiOutShortMsg(midiplayerhMidi,midiplayerLastNoteOff);

    midiplayerLastNoteOff = 0;

}


void  tone (int pin,int Freq)
{
    int Voice=81;
    int Note = 0;
    int Phrase = 0;
    int Tempo=0;
    int Vol=127;

    if(midiplayerEnabled==0)
    {
        midiplayerEnabled=1;
        midiOutOpen(&midiplayerhMidi,0,0,0,CALLBACK_NULL);

        midiOutShortMsg(midiplayerhMidi,(256*Voice)+192);
    }
    else
    {
        noTone(pin);
    }
    // convert frequency to midi note
    Note = (int)midiplayerRound((log((float)Freq)-log(440.0))/log(2.0)*12+69,0);
    Phrase = (Vol*256+Note)*256+144;
    midiplayerLastNoteOff=(0*256+Note)*256+144;

    midiOutShortMsg(midiplayerhMidi,Phrase);

}
