#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

extern int play_rtttl(void);
extern int play_rtttl_stop();
extern int play_rtttl_start(int song);
extern void play_rtttl_start_no_repeat(int);
extern int play_rtttl_song();
extern unsigned long songTime;
extern int songIndex;

#define soundLoose 6
#define soundWin 7
#define soundPing 8
#endif // AUDIO_H_INCLUDED
