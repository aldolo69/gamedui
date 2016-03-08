#include <windows.h>
#include <stdlib.h>
double millsUtilPCFreq = 0.0;
__int64 millsUtilCounterStart = 0;
int millsUtilInit=0;


unsigned long millis()
{
    LARGE_INTEGER li;

    if(millsUtilInit==0)
    {
        millsUtilInit=1;
        QueryPerformanceFrequency(&li);

        millsUtilPCFreq = double(li.QuadPart)/1000.0;

        QueryPerformanceCounter(&li);
        millsUtilCounterStart = li.QuadPart;
        return 0;
    }



    QueryPerformanceCounter(&li);
    return double(li.QuadPart-millsUtilCounterStart)/millsUtilPCFreq;

}

void randomSeed(int seed)
{
    srand(seed);
}

int random(int iMax)
{
    return rand()%iMax;
}


void *memcpy_P(void *to, const void *fr, int sz)
{
   return  memcpy(to,fr,sz);
}

int strncpy_P (char *to, const char *fr, int sz)
{
     strncpy(to,fr,sz);
     return 0;
}
