#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <windows.h>
extern unsigned char display[4][8];
void windowsShowDisplay()
{
//display[4][8];
//pulisco il video e mando 16*16 caratteri

   // system("cls");
   	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
COORD coord={0,0};
static int cnt=0;

	// Tell the user how to stop
	SetConsoleTextAttribute( hstdout, FOREGROUND_RED );
	SetConsoleCursorPosition(hstdout,coord);
    for(int riga=0; riga<2; riga++)
    {
        for(int scanline=0; scanline<8; scanline++)
        {
            for(int colonna=0; colonna<2; colonna++)
            {
                unsigned char ucBits=display[riga+colonna*2][scanline];
                for(unsigned char electronicbean=128; electronicbean!=0; electronicbean>>=1)
                {
                    if(ucBits&electronicbean)
                    {
                        printf("*");
                    }
                    else
                    {
                        printf(".");
                    }
                }

            }
             printf("\n");
        }
    }

    switch(cnt){case 0:std::cout<<'|';break;
    case 1:std::cout<<'/';break;
    case 2:std::cout<<'-';break;
    case 3:std::cout<<'\\';break;
    }
 if(++cnt==4)cnt=0;

}
