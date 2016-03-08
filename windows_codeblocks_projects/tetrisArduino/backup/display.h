/*

questo file gestisce quello che sarà il video di tetris, ossia una matrice di 16*16 pixel divisa in 4
blocchi per le 4 matrici 8*8


+-----------+ +-----------+
display[0][0] display[2][0]

display[0][7] display[2][7]
+-----------+ +-----------+

+-----------+ +-----------+
display[1][0] display[3][0]

display[1][7] display[3][7]
+-----------+ +-----------+

*/
#ifndef _display_h_
#define _display_h_


extern unsigned char icons[][8];
extern void displayClear();
extern void displayShowIcon(int iconIndex, int iconPosition);

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


#endif // _display_h_
