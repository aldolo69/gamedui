#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <windows.h>
unsigned char displayW[4][8];
void windowsShowDisplay()
{
//display[4][8];
//pulisco il video e mando 16*16 caratteri
    /*

    copy bytes from display memory to max chips. remember:

    display[0]-->chip0 0000-->0000
    display[1]-->chip2 0001-->0010
    display[2]-->chip1 0010-->0001
    display[3]-->chip3 0011-->0011

    -->swap bit 0 and 1! too complex. better a switch command

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



    // system("cls");
    HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
    COORD coord= {0,0};
    static int cnt=0;

    // Tell the user how to stop
    SetConsoleTextAttribute( hstdout, FOREGROUND_RED );
    SetConsoleCursorPosition(hstdout,coord);

    for(int colonna=0; colonna<2; colonna++)
    {
        for(int scanline=0; scanline<8; scanline++)
        {
            for(int riga=0; riga<2; riga++)
            {
                unsigned char ucBits=displayW[riga+colonna*2][7-scanline];
                for(unsigned char electronicbeam=128; electronicbeam!=0; electronicbeam>>=1)
                {
                    if(ucBits&electronicbeam)
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

    switch(cnt)
    {
    case 0:
        std::cout<<'|';
        break;
    case 1:
        std::cout<<'/';
        break;
    case 2:
        std::cout<<'-';
        break;
    case 3:
        std::cout<<'\\';
        break;
    }
    if(++cnt==4)cnt=0;

}
