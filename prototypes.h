/*
Name: Seenan Bunni
prototypes.h

Header file that holds all the function
prototypes for the CPU.
 */

#include <stdint.h> //This is for uintX_t types

void menu (void);
char menuChoice(void);
void holdDisplay();
int loadFile(void *, uint16_t);
void writeFile(void *);
void memDump(void *, uint16_t, uint16_t);
void memMod(void *, uint16_t);

void dumpRegisters(void);
void zero(void);
void fetch(void *);
void trace(void *, uint8_t, uint32_t);

void go(void *);
void execute(uint16_t, void *);

void immediate(uint16_t);
void data(uint16_t);
void loadstore(uint16_t, void *);

void pushpull(uint16_t, void *);
void push(uint32_t, uint32_t *, void *);
void pull(uint32_t *, uint32_t *, void *);

void condbr(uint16_t);
void uncondbr(uint16_t, void *);

void SCZ(uint32_t, uint32_t);
void SZ(void);

uint8_t isREGMASK(uint16_t);

void toggle(uint16_t, uint32_t *);
void set(uint16_t, uint32_t *);
void clear(uint16_t, uint32_t *);

int iscarry(unsigned long, unsigned long, unsigned);
