//found at https://github.com/flickerfly/Music_Box/blob/master/play_rtttl.ino

#include <Arduino.h>
#include "tetris.h"
#include "keyboard.h"
#include "display.h"

int tonePin = 7;

#define OCTAVE_OFFSET 0

// These values can also be found as constants in the Tone library (Tone.h)
int notes[] = { 0,
                262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
                523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
                1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
                2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
              };

//add here a nokia tone. remember to update the "song" array below
const char song0[] PROGMEM =
    "korobyeyniki:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a";
const char song1[] PROGMEM =
    "Hitchcoc:d=4,o=5,b=200:16c,16p,16f4,8p,8f,32g,32p,16f,32p,16e,32p,16d,32p,16e,8p,16f,32p,16g,8p.,16c,16p,16f4,8p,8f,32g,32p,16f,32p,16e,32p,16d,32p,16e,8p,16f,32p,16g,8p.,16c,16p,16f4,8p,16g#,32p,8c6,16p,16a#,32p,16g#,8p,16c6,32p,8d#6,16p,16c#6,32p,16c6,8p,16d#6,32p,8g6,16p,16f6,32p,16e6,32p,16c#6,32p,16c6,32p,16a#,32p,16g#,32p,16g,32p,8f4";
const char song2[] PROGMEM =
    "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char song3[] PROGMEM =
    "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
const char song4[] PROGMEM =
    "JBRock:d=8,o=6,b=125:g5,e,d,c,2g5,g5,e,d,c,2a5,a5,f,e,d,b5,g5,b5,d,g.,16g,f,d,2e,g5,e,d,c,2g5,16f#5,g5,e,d,c,2a5,a5,f,e,d,g,16g,16f#,16g,16f#,16g,16g#,a.,16g,e,d,4c,4g,e,e,e.,16d#,e,e,e.,16d#,e,g,c.,16d,2e,f,f,f.,16f,f,e,e,16e,16e,e,d,d,e,2d";
const char song5[] PROGMEM =
    "Santa Clause is Coming Tonight:d=4,o=5,b=200:g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,f,2f,e,g,c,e,d,2f,b4,1c,p,g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,f,f,e,g,c,e,d,2f,b4,1c,p,c6,d6,c6,b,c6,a,2a,c6,d6,c6,b,c6,2a.,d6,e6,d6,c#6,d6,b,b,b,8b,8c6,d6,c6,b,a,g,p,g.,8g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,8f,2f,e,g,c,e,d,2f,d6,1c6.";


const  char *  const song[] =
{
    song0, song1, song2, song3,song4,song5
};


//scorro la canzone da questo indice in avanti. quando la nota che sto suonando finisce passo alla successiva richiamando play_rtttl
//la funzione decide per quanto tempo devo tenere la nota. finita la nota
//si passa alla nota successiva
int songIndex = 0; //indice della nota che sto suonando adesso
int songFreq = 0; //frequenza della nota che sto suonando adesso
unsigned long songTime = 0; //durata della nota che sto suonando adesso
int songbpm = 63;
unsigned char songdefault_dur = 4;
unsigned char songdefault_oct = 6;
long songwholenote;
int songNumber = 0;

unsigned char songEnabled = 0;



char songCache[8];
int songCacheIndex = -99;
int songCacheNumber = -99;


char * songCacheRead(int iNumber, int iIndex)
{
  if (songCacheNumber != iNumber) //changed song?
  {
    songCacheIndex = -99; //discard old cache
  }

  if (iIndex >= (songCacheIndex + 8)) //out of boundaries?
  {
    songCacheIndex = -99; //discard old cache
  }
  if (songCacheIndex < 0)
  {
    songCacheIndex = iIndex;
    memcpy_P((void *)songCache, (void *)(song[songNumber] + iIndex), 8);
  }

  return songCache + (iIndex - songCacheIndex);

}

#define isdigit(n) (n >= '0' && n <= '9')

int play_rtttl()
{

  char *p;//next byte!!
  int num;

  long duration;
  unsigned char note;
  unsigned char scale;

  if (songEnabled == 0) return 0;


  p = songCacheRead(songNumber, songIndex);
  if (*p == 0) return 1;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  if (songIndex == 0) //begin of song
  {

    songbpm = 63;
    songdefault_dur = 4;
    songdefault_oct = 6;


    while (*p != ':')
    {
         // ignore name
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }
                         // skip ':'
    songIndex++;
    p = songCacheRead(songNumber, songIndex);
    // get default duration
    if (*p == 'd')
    {
      
                    // skip "d="
      songIndex++;
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
      num = 0;
      while (isdigit(*p))
      {
        num = (num * 10) + (*p++ - '0');
        songIndex++;
        p = songCacheRead(songNumber, songIndex);
      }
      if (num > 0) songdefault_dur = num;
                         // skip comma
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }

    // Serial.print("ddur: "); Serial.println(songdefault_dur, 10);

    // get default octave
    if (*p == 'o')
    {
                    // skip "o="
      songIndex++;
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
      num = *p++ - '0';
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
      if (num >= 3 && num <= 7) songdefault_oct = num;
                          // skip comma
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }

    //Serial.print("doct: "); Serial.println(songdefault_oct, 10);

    // get BPM
    if (*p == 'b')
    {
                   // skip "b="
      songIndex++;
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
      num = 0;
      while (isdigit(*p))
      {
        num = (num * 10) + (*p++ - '0');
        songIndex++;
        p = songCacheRead(songNumber, songIndex);
      }
      songbpm = num;
                         // skip colon
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }

    //Serial.print("bpm: "); Serial.println(songbpm, 10);

    // BPM usually expresses the number of quarter notes per minute
    songwholenote = (60 * 1000L / songbpm) * 4;  // this is the time for whole note (in milliseconds)

    //Serial.print("wn: "); Serial.println(songwholenote, 10);
  }


  // now begin note loop
  while (*p)
  {
    // first, get note duration, if available
    num = 0;
    while (isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }

    if (num) duration = songwholenote / num;
    else duration = songwholenote / songdefault_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch (*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;
    songIndex++;
    p = songCacheRead(songNumber, songIndex);
    // now, get optional '#' sharp
    if (*p == '#')
    {
      note++;
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }

    // now, get optional '.' dotted note
    if (*p == '.')
    {
      duration += duration / 2;
       songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }

    // now, get scale
    if (isdigit(*p))
    {
      scale = *p - '0';
       songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }
    else
    {
      scale = songdefault_oct;
    }

    scale += OCTAVE_OFFSET;

    if (*p == ',')
    {
             // skip comma for next note (or we may be at the end)
      songIndex++;
      p = songCacheRead(songNumber, songIndex);
    }
    // now play the note

    if (note)
    {
    //  Serial.print("Playing: ");
    //  Serial.print(scale, 10); Serial.print(' ');
    //  Serial.print(note, 10); Serial.print(" (");
    //  Serial.print(notes[(scale - 4) * 12 + note], 10);
    //  Serial.print(") ");
    //  Serial.println(duration, 10);

      songFreq = notes[(scale - 4) * 12 + note];
      tone(tonePin, songFreq);
      //next time this function will be called...
      songTime = millis() + (duration * 12) / 10; //      delay(duration);
    }
    else
    {
      songFreq = 0;
      //Serial.print("Pausing: ");
      //Serial.println(duration, 10);
      //                   delay(duration);
      noTone(tonePin);
      songTime = millis() + (duration * 12) / 10; //
    }

 
    return 0;

  }



  return 1;

}


int play_rtttl_stop()
{
  noTone(tonePin);
  songEnabled = 0;
  return 0;
}

int play_rtttl_start(int song)
{
  noTone(tonePin);
  songNumber = song;
  songIndex = 0;
  songEnabled = 1;
  return 0;
}

