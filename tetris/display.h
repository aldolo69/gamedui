/*

this is the tetris display buffer. for easy of programming 0,0 origin is in the top left corner
0,0--->15,0
|
|
v
0,15   15,15


+-----------+ +-----------+
display[0][0] display[2][0]

display[0][7] display[2][7]
+-----------+ +-----------+

+-----------+ +-----------+
display[1][0] display[3][0]

display[1][7] display[3][7]
+-----------+ +-----------+


for easy building the max7219 chips are physically arranged in a different manner

chip0   chip1
  addr7   addr7
  .       .
  .       .
  addr0   addr0

chip2   chip3
  addr7   addr7
  .       .
  .       .
  addr0   addr0


so:
display[0]-->chip0
display[1]-->chip2
display[2]-->chip1
display[3]-->chip3

more over, byte are swapped top<->bottom
display[0][0]-->chip0[7]
display[0][1]-->chip0[6]
display[0][2]-->chip0[5]
display[0][3]-->chip0[4]
display[0][4]-->chip0[3]
display[0][5]-->chip0[2]
display[0][6]-->chip0[1]
display[0][7]-->chip0[0]



*/
#ifndef _display_h_
#define _display_h_
 

//extern unsigned char PROGMEM icons[][8];
extern unsigned char iconCache[8];
extern void displayClear();
extern void displayShowIcon(int iconIndex, int iconPosition);
extern void displayReadCache(int icon);



//0=set
//1=toggle
//2=overwrite
//4=collision=draw nothing. check collision
extern char displayShowIconXY(int iconIndex, int x,int y,char operation);
extern unsigned char display[4][8];


//0=set bits
//1=toggle bits
//2=overwrite full bytes
//3=reset bits

#define _draw_set 0
#define _draw_toggle 1
#define _draw_overwrite 2
#define _draw_reset 3
#define _draw_collision 4
extern void displayPixel(int x,int y,char operation);
extern void displayLineDx(int x,int y,int len,char operation);
extern void displayLineDy(int x,int y,int len,char operation);

#define _icon_sound 0
#define _icon_no_sound 1
#define _icon_t 2
#define _icon_v 3
#define _icon_0 4
#define _icon_play 14
#define _icon_arrow 15
#define _icon_tetris_1 16


extern void displayInitLed();
extern void displayCopyToLed();


#endif // _display_h_
