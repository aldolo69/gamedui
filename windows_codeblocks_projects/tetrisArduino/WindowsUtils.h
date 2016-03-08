
extern unsigned long millis(void);

extern int random(int  );

extern void randomSeed(int seed);


//PROGMEM simulation
typedef  char prog_char;
typedef  unsigned char prog_uchar;
#define PROGMEM

void *memcpy_P(void *, const void *, int);
int strncpy_P (char *, const char *, int );
