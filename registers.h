
//Registers

unsigned long int r[16];
unsigned long int mbr;
unsigned long int mar;
unsigned long int ALU;
static unsigned long int ir;
static unsigned long int ccr;

static unsigned int tempsign;
static unsigned int tempzero;
static unsigned int tempcarry;

static int irflag = 0;
static int stopflag = 0;

#define SP r[13] //Stack Pointer definition
#define LR r[14] //Link Register definition
#define PC r[15] //Program Counter definition

