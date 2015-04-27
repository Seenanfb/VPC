//All Registers, including the IR/Stop Flags

unsigned long r[16];
unsigned long mbr;
unsigned long mar;
unsigned long ALU;
static unsigned long ir;
static unsigned long ccr;

static int irflag = 0;
static int stopflag = 0;

#define SP r[13] //Stack Pointer definition
#define LR r[14] //Link Register definition
#define PC r[15] //Program Counter definition
