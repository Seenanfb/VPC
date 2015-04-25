void menu (void);
char menuChoice(void);
void holdDisplay();
int loadFile(void *, unsigned int);
void writeFile(void *);
void memDump(void *, unsigned, unsigned);
void memMod(void *, unsigned);

unsigned long int ir0(unsigned long irx);
unsigned long int ir1(unsigned long irx);

void dumpRegisters(void);
void zero(void);
void fetch(void *);
void trace(void);

void go(void *);
void execute(unsigned int, void *);

void immediate(unsigned int);
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

