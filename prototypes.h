//Prototypes

#include <stdint.h> //This is for uintX_t types

void menu (void);
char menuChoice(void);
void holdDisplay();
int loadFile(void *, unsigned int);
void writeFile(void *);
void memDump(void *, unsigned, unsigned);
void memMod(void *, unsigned);

void dumpRegisters(void);
void zero(void);
void fetch(void *);
void trace(void *, int, unsigned long);

void go(void *);
void execute(uint16_t, void *);

void immediate(uint16_t);
void data(unsigned int);
void loadstore(unsigned int, void *);

void pushpull(unsigned int, void *);
void push(unsigned long, unsigned long *, void *);
void pull(unsigned long *, unsigned long *, void *);

void condbr(unsigned int);
void uncondbr(unsigned int, void *);

void SCZ(unsigned int, unsigned int);
void SZ(void);

unsigned int isREGMASK(unsigned int);

void toggle(unsigned int, unsigned long *);
void set(unsigned int, unsigned long *);
void clear(unsigned int, unsigned long *);

unsigned int getIMM(unsigned int);
unsigned int getRD(unsigned int);
unsigned int getRN(unsigned int);

int iscarry(unsigned long, unsigned long, unsigned);
