/*
Name: Seenan Bunni
Program: This program is essentially a virtual ARM-based CPU with
a developed terminal user-interface.

As well, it integrates some assembly language functions inside such as
determining the instruction type of assembly language instructions.

 */

//Main includes

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#include "prototypes.h" //Prototypes header
#include "define.h"     //Holds all defines (header file)
#include "registers.h"  //File that holds all register variables

//Some general defines

#define MAXSIZE 16384
#define MAX32   0xFFFFFFFF

#define false 0
#define true !0
#define sixteen 0x10 //Magic numbers? Not anymore.

/* Assumptions

   long is 4 bytes

   Assumptions end */


unsigned char mem[MAXSIZE]; //Memory of file stored here..just because.

//Main program

int main(int argc, char * argv[],char * envp[])
{
    char choice = '\0';
    int max = MAXSIZE;
    unsigned int read;
    unsigned offset, length;

    fprintf(stdout,"Seenan Bunni - 821 622 107\n\n");

    zero();     //Nothing should be in the registers.

    for(;;){
        choice=menuChoice();
        if(choice == 'd' || choice == 'D'){
            fprintf(stdout,"Enter offset: ");
            fscanf(stdin, "%X",  &offset);
            fprintf(stdout,"\nEnter length: ");
            fscanf(stdin, "%X",  &length);
            //  if(offset+length > MAXSIZE)
            //          perror("
            memDump(mem, offset, length);
        }
        else if(choice == 'g' || choice == 'G'){
            go(mem);
        }
        else if(choice == 'l' || choice == 'L'){
            read = loadFile(mem, max);
            if(read!=-1)
                fprintf(stdout,"Bytes successfully read: %d in Decimal, 0x%X in Hex.\n", read, read);
            else
                fprintf(stdout,"No bytes read\n");
        }
        else if(choice == 'm' || choice == 'M'){
            fprintf(stdout,"Enter offset: ");
            fscanf(stdin, "%X",  &offset);
            memMod(mem, offset);
        }
        else if(choice == 'r' || choice == 'R'){
            dumpRegisters();
        }
        else if(choice == 't' || choice == 'T'){
            trace(mem, irflag, ir);
        }
        else if(choice == 'w' || choice == 'W'){
            writeFile(mem);
        }
        else if(choice == 'z' || choice == 'Z'){
            zero();
        }
        else if(choice == '?' || choice == 'h' || choice == 'H'){
            menu();
        }
        else if(choice == 'q' || choice == 'Q'){
            fprintf(stdout,"Exiting...\n");
            return 0;
        }
        else{
            fprintf(stdout,"\n");
            fprintf(stdout,"Error...Enter valid choice\n");
        }
    }

    return 0;
}

//menu

void menu (void){
    fprintf(stdout,"\n");
    fprintf(stdout,"d   dump memory\n");
    fprintf(stdout,"g   go - run the program\n");
    fprintf(stdout,"l   load a file into memory\n");
    fprintf(stdout,"m   memory modify\n");
    fprintf(stdout,"r   display registers\n");
    fprintf(stdout,"t   trace - execute one instruction\n");
    fprintf(stdout,"w   write file\n");
    fprintf(stdout,"z   reset all registers to zero\n");
    fprintf(stdout,"?, h        display list of commands\n");
    fprintf(stdout,"q   quit\n");
    fprintf(stdout,"\n");
}

/*
   menuChoice

   Gets the menu input. That's all.

 */


char menuChoice(void)
{
    char choice = '\0';
    fprintf(stdout,"> ");
    fscanf(stdin, " %c", &choice);
    holdDisplay();
    return choice;
}

/*
   loadFile

   Loads a file into memory and reads it, truncating it if it's above 16K,
   AND IF IT EXISTS.

 */
int loadFile(void *memory, uint16_t max)
{


    char buff[150];
    FILE *thefile;
    int readin=0;
    struct stat stats;
    int filemax=0;
    int status;


    memset(buff, 0, sizeof buff);

    fprintf(stdout,"Enter name of a file...\n");
    fscanf(stdin, "%[^\n]", buff);

    thefile= fopen(buff, "rb");
    if(!thefile){
        perror("Error");
        return -1;
    }
    else
        fseek(thefile, 0L, SEEK_END);
    filemax = ftell(thefile);
    fseek(thefile, 0L, SEEK_SET);

    readin = fread(memory, (size_t) 1, (1*MAXSIZE), thefile);

    if(readin == MAXSIZE)
    {
        fprintf(stdout,"File larger than max file size (16KB)\n");
        fprintf(stdout,"File probably truncated.\n");
        return readin;
    }
    return readin;
}


/*

   writeFile

   Writes amount of bytes specified, to specified file


 */

void writeFile(void *memory)
{
    //Some error checking included
    char fname[150]; //People sometimes like writing "aaaaaaaaaaaaaaaaaaaaaaaaaaa" as a filename (ridiculous/long filenames)
    char buff[100];
    int bytes=0;
    FILE *f;
    int writein=0;
    char sizecheckcmd[50] = "ls -s ";
    FILE *p;
    char pbuff[100];

    memset(fname, 0, sizeof fname);
    memset(buff, 0, sizeof buff);

    fprintf(stdout,"Enter file name...\n");
    fscanf(stdin, "%[^\n]", fname);

    fprintf(stdout,"How many bytes should be written to file?\n");
    fscanf(stdin, "%d", &bytes);
    while(bytes > MAXSIZE)
    {
        fprintf(stdout,"Error: Enter amount lower than or equal to %d\n", MAXSIZE);
        fscanf(stdin, "%d", &bytes);
    }

    f = fopen(fname, "wb+");
    writein = fwrite(memory, (size_t) 1, bytes, f);
}

/*
   memDump

   Dumps the memory data from the offset,
   to a specified length

 */

void memDump(void *memptr, uint16_t offset, uint16_t length)
{
    //isprint
    uint16_t i, j;
    char *ptr;
    int lengthOfRow = sixteen;
    ptr = memptr;

    for(i = offset; i < (offset + length); i+= lengthOfRow)
    {
        fprintf(stdout,"%04X\t", i);

        for(j = i; j < (i + lengthOfRow); j++)
        {
            fprintf(stdout,"%02X  ", (uint8_t) *(ptr + j));
            if(j == (offset+length))
                break;
        }

        //fprintf(stdout,"%X\n", *(ptr));
        fprintf(stdout,"\n\t");

        for(j = i; j < (i + lengthOfRow); j++)
        {
            if(isprint((int) *(ptr+j)))
                fprintf(stdout," %c  ", *(ptr+j));
            else
                fprintf(stdout," .  ");
            if(j == (offset+length))
                break;
        }

        if(j == (offset+length))
            break;

        fprintf(stdout,"\n");
    }

    fprintf(stdout,"\n");
    return;
}

/*
   memMod

   Modifies the memory in real-time. Exit by inputting "."
   without the quotes.


 */


void memMod(void *memory, uint16_t offset)
{

    char buff[123];
    int temp;
    int max = 0xFF;

    fprintf(stdout,"Enter starting address (enter . to exit): \n");

    if(offset >= MAXSIZE)
        offset=0;

    holdDisplay();

    buff[(strlen(buff) - 1)] = '\0';

    while(1) {
        fprintf(stdout,"\n");
        fprintf(stdout,"%04X %X>  ", offset, *((unsigned char*) memory+offset) );
        fgets(buff, 4, stdin);


        if(buff[0] == '.'){
            break;
        }

        if(sscanf(buff, "%X", &temp) == 0)
            perror("Non-hex digit found");
        else{
            if(temp > max)
                fprintf(stdout,"Out of range...\n");

        }

        fflush(stdout);

        temp = (uint8_t) strtoul(buff, NULL, 0x10);

        *((uint8_t *) memory + offset) = temp;

        if(++offset == MAXSIZE)
            return;
    }


    return;


}

/*
   dumpRegisters

   Displays all registers in a format.

 */
void dumpRegisters()
{
    uint8_t i = 0;

    while(i < sixteen)
    {
        fprintf(stdout,"R%02d:  0x%08X ", (i), r[i]);
        i++;
        if(i == 13)
            break;

        if(i % 4 == 0)
            fprintf(stdout,"\n");
    }

    fprintf(stdout,"\n");
    fprintf(stdout,"\nSP: 0x%08X \tCCR: %d%d%d (SCZ)  MBR: 0x%08X  MAR: 0x%08X\n", SP, isCARRYset(ccr), isZEROset(ccr), isSIGNset(ccr), mbr, mar);
    fprintf(stdout,"LR: 0x%08X \tIR: 0x%08X  IR0: 0x%04X      IR1: 0x%04X\n", LR, ir, IR(ir), IR1(ir));
    fprintf(stdout,"PC: 0x%08X \tStop Flag: %d    Active IR Flag: %d\n", PC, isSTOPset(stopflag), IRoneORtwo(irflag));
    //Made the display more compact
    fprintf(stdout,"\n");

}

/*
   zero

   Clears all registers/makes them all = 0.

 */

void zero()
{

    int i;
    //sixteen = 0x10
    for(i=0; i < 16; i++)
        r[i] = 0;

    mbr = 0;
    mar = 0;
    ALU = 0;
    ir = 0;
    ccr = 0;

    irflag = 0;
    stopflag = 0;
}

/*
   fetch

   Fetches the next instruction from memory.

 */


void fetch(void *memory)
{
    //Concept
    //MAR <- PC
    //MBR <- mem[MAR]
    //IR <- MBR
    //PC <- PC+1(instruction) (+4?)

    //It seems to work even if PC+=4 is before ir=mbr
    //rev 2

    mbr=0;
    mar=PC;

       mbr+=*((uint8_t *) memory + (mar++));
       mbr=(mbr << aByte);

       mbr+=*((uint8_t *) memory + (mar++));
       mbr=(mbr << aByte);

       mbr+=*((uint8_t *) memory + (mar++));
       mbr=(mbr << aByte);

       mbr+=*((uint8_t *) memory + (mar++));

    //unsigned char works the same as uint8_t


//    pull(&mbr, &mar, memory);
//    Works the same.

    ir=mbr;

    PC+=4;



    return;
}
/*
   trace

   Traces through the program code, one line at a time
 */
void trace(void *memory, uint8_t irflagx, uint32_t irx)
{

    uint32_t irnum  = IR1(irx);
    uint32_t irnum1 = IR(irx);

    if(irflagx){
        execute(irnum, memory);
    }
    else {
        fetch(memory);
        execute(irnum1, memory);
    }
    dumpRegisters();
    return;
}


/*
   execute

   Deciphers the instruction type, and sends it
   to the appropriate function to work with
   that specific instruction type


   The stop flag basically tells the
   program to stop executing.

 */
void execute(uint16_t instr, void *memory)
{

    if(IMMinst(instr))
        immediate(instr);
    if(DATAinst(instr))
        data(instr);
    if(LSinst(instr))
        loadstore(instr, memory);
    if(PSHPLLinst(instr))
        pushpull(instr, memory);
    if(CONDBRinst(instr))
        condbr(instr);
    if(UNBRinst(instr))
        uncondbr(instr, memory);


    if(STOPinst(instr)){
        set(STOPF, (uint32_t *) &stopflag);
        return;
    }

    toggle(IRF, (uint32_t *) &irflag);


    return;
}

/*
   go

   Like Trace, it goes through the program code (runs the code)
   But unlike trace, it goes through the *whole* code.
   Not one line at a time.

 */
void go(void *memory)
{

    if(stopflag)
        return;

    while(!stopflag){
        if(irflag){
            execute(IR1(ir), memory);
        }
        else {
            fetch(memory);
            execute(IR(ir), memory);
        }
    }
    dumpRegisters();
    return;
}

/*

   immediate

   If the instruction type is deemed to be
   immediate, it's sent here to perform the
   necessary immediate operations for the
   instruction.


 */

void immediate(uint16_t instr)
{
    uint16_t op = IMMgetopcode(instr);
    uint8_t rd = RD(instr);
    uint16_t imm = IMMED(instr);
    //uint_t not necessary
    //But it still works that way
    //Was mainly for testing purposes

    switch(op)
    {
        case IMMmov:
            ALU = imm;
            SZ();
            r[rd] = ALU;
            break;
        case IMMcmp:
            ALU = r[rd] + ~imm + 1;
            SZ();
            break;
        case IMMadd:
            ALU = r[rd] + imm;
            SCZ(r[rd], imm);
            r[rd] = ALU;
            break;
        case IMMsub:
            ALU = r[rd] + ~imm + 1;
            SCZ(r[rd], ~(imm + 1));
            r[rd] = ALU;
            break;
    }

    return;
}


/*
   data

   Like immediate, if the instruction type
   is deemed to be a data instruction,
   it's instead sent here.

   Performs register to register operation.

 */


void data(uint16_t instr)
{

    uint32_t i;

    uint8_t rd = RD(instr);
    uint8_t rn = RN(instr);
    uint16_t op = dataGETOP(instr);

    switch(op)
    {
        case ANDcode:
            ALU = r[rd] & r[rn];
            SZ();
            r[rd] = ALU;
            break;
        case EORcode:
            ALU = r[rd] ^ r[rn];
            SZ();
            r[rd] = ALU;
            break;
        case SUBcode:
            ALU = r[rd] + ~r[rn] + 1;
            SCZ(rd, ~(rn+1));
            r[rd] = ALU;
            break;
        case SXBcode:
            ALU = (aByteMASK & r[rn]);
            SZ();
            r[rd] = ALU;
            break;
        case ADDcode:
            ALU = r[rd] + r[rn];
            SCZ(rd, rn);
            r[rd] = ALU;
            break;
        case ADCcode:
            ALU = r[rd] + r[rn] + isCARRYset(ccr);
            SCZ(rd, rn);
            r[rd] = ALU;
            break;
        case LSRcode:
            ALU = r[rd] >> (r[rn] - 1);
            if(hasLSB(ALU))
                set(CARRYF, &ccr);
            ALU = ALU >> 1;
            SZ();
            r[rd] = ALU;
            break;
        case LSLcode:
            ALU = r[rd] << (r[rn] - 1);
            if(hasMSB(ALU))
                set(CARRYF, &ccr);
            ALU = ALU << 1;
            SZ();
            r[rd] = ALU;
            break;
        case TSTcode:
            ALU = r[rd] & r[rn];
            SCZ(rd, rn);
            break;
        case TEQcode:
            ALU = r[rd] ^ r[rn];
            SCZ(rd, rn);
            break;
        case CMPcode:
            ALU = r[rd] + ~r[rn] + 1;
            SCZ(rd, rn);
            break;
        case RORcode:
            ALU = r[rd];

            for(i = 0; i < r[rn]; i++){
                if(hasLSB(ALU))
                    set(CARRYF, &ccr);

                ALU = ALU >> 1;

                if(isCARRYset(ccr))
                    ALU |= MostSBmask;
            }
            SZ();
            r[rd] = ALU;
            break;

        case ORRcode:
            ALU = r[rd] | r[rn];
            SZ();
            r[rd] = ALU;
            break;

        case MOVcode:
            ALU = r[rn];
            SZ();
            r[rd] = ALU;
            break;
        case BICcode:
            ALU = ~(r[rd] & r[rd]);
            SZ();
            r[rd] = ALU;
            break;

        case MVNcode:
            ALU = ~r[rn];
            SZ();
            r[rd] = ALU;
            break;
    }

    ALU=0;
    return;
}


/*
   loadstore

   Performs the necessary load or store
   function, if the instruction type
   is deemed to be a load or store

 */

uint32_t load(uint32_t marx, void *memory)
{
        unsigned int i;

        mar = marx;

        for(i = 0; i < 4; i++){
        mbr = mbr << aByte;
        mbr += *((unsigned char *) memory + mar);
        }

        return mbr;

}

void store(uint32_t marx, uint32_t mbrx, void *memory)
{

mar = marx;
mbr = mbrx;

*((unsigned char *) memory + mar++) = (unsigned char)(mbr >> shift3 & aByteMASK);

*((unsigned char *) memory + mar++) = (unsigned char)(mbr >> shift2 & aByteMASK);
*((unsigned char *) memory + mar++) = (unsigned char)(mbr >> shift1 & aByteMASK);

*((unsigned char *) memory + mar) = (unsigned char)(mbr & aByteMASK);

}
void loadstore(uint16_t instr, void *memory)
{
    uint8_t rn = RN(instr);
    uint8_t rd = RD(instr);

    //LOAD

    if(LSload(instr))
    {
        if(LSdword(instr)){
//          pull(&ALU, ((uint32_t *) &rn), memory);
                r[rd] = load(r[rn], memory);
                r[rd] = r[rd] & aByteMASK;
        }
        else
//          pull(&ALU, ((uint32_t *) &rn), memory);
                r[rd] = load(r[rn], memory);
    }

    //STORE

    else
    {
        mbr = r[rd];

        if(LSdword(instr)){
//          push(mbr, ((uint32_t *) &rn), memory);
        mar = r[rn];
        mbr = r[rd];
        *((unsigned char *) memory + mar) = (unsigned char) mbr & aByteMASK;
        }
        else
//          *((uint8_t *) memory + rn) = (uint8_t) (r[rd] & aByteMASK);
        store(r[rn], r[rd], memory);
    }
}

/*
   pushpull

   This function handles the push and pull instruction.
   It will store or load register values from memory
   if need be.


 */

void pushpull(uint16_t instr, void *memory)
{

    uint8_t i, j;
    uint8_t maskset[] = { regmask1, regmask2, regmask3,
        regmask4, regmask5, regmask6,
        regmask7, regmask8 };

    uint8_t REGset = PPGETreg(instr);


    //PUSH

    if(PPush(instr)){
        if(PPextra(instr))
            push(LR, &mar, memory);
        if(PPhigh(instr))
            for(i = HIGHreg, j = 0; i < sixteen; i++)
            {
                if(maskset[j++] & REGset)
                {
                    SP = (SP - regsize);
                    push(r[i], &mar, memory);
                }
            }
        else
            for(i = 0; i <= LOWreg; i++)
                if(maskset[i] & REGset){
                    SP = (SP - regsize);
                    push(r[i], &mar, memory);
                }

    }

    //PULL

    else {

        if(PPhigh(instr))
            for(i = HIGHreg, j = sixteen; i >= HIGHreg; i--)
                if(maskset[j--] & REGset)
                {
                    pull(&mbr, &mar, memory);
                    SP = (SP + regsize);
                }
                else
                    for(i = LOWreg; i <= 0; i--)
                        if(maskset[i] & REGset)
                        {
                            pull(&mbr, &mar, memory);
                            SP = (SP + regsize);
                        }

        if(PPextra(instr))
            pull(&PC, &mar, memory);

    }

    return;

}


/*

   condbr

   The function to handle conditional branch
   instructions.

 */



void condbr(uint16_t instr)
{



    int8_t addr = CONDGETaddr(instr);

    ALU = PC + addr;

if(irflag != 0){
        irflag = 0;
        ALU = ALU + ~2 + 1;
        }
PC = ALU;

    switch(CONDGETop(instr))
    {

        case CONDal:
            PC += addr;
            break;
        case CONDeq:
            if(isZEROset(ccr))
                PC += addr;
            break;
        case CONDne:
            if(!isZEROset(ccr))
                PC+=addr;
            break;
        case CONDcs:
            if(isCARRYset(ccr))
                PC+=addr;
            break;
        case CONDcc:
            if(!isCARRYset(ccr))
                PC+=addr;
            break;
        case CONDmi:
            if(isSIGNset(ccr))
                PC+=addr;
            break;
        case CONDpl:
            if(!isSIGNset(ccr))
                PC+=addr;
            break;
        case CONDhi:
            if(isCARRYset(ccr) && !isSIGNset(ccr))
                PC+=addr;
            break;
        case CONDls:
            if(!isCARRYset(ccr) && isSIGNset(ccr))
                PC+=addr;
            break;
    }

}


/*

   uncondbr


   This function will work with the unconditional branches
   as opposed to conditional branches.

 */

void uncondbr(uint16_t instr, void *memory)
{


    uint16_t addr = UNBRGEToff(instr);

    mar=addr;

    if(isUNBRA(instr)){
        PC = (uint32_t) addr;
        return;
    }

    if(isUNBRL(instr)){
        push(LR, &mar, memory);
        PC = (uint32_t) addr;
        return;
    }
    return;
}

/*
   push


   Pushes a full register into memory.

 */

void push(uint32_t regist, uint32_t *marx, void *memory)
{
    *((uint8_t *) memory + (*marx++)) = (uint8_t) byte4(regist);
    *((uint8_t *) memory + (*marx++)) = (uint8_t) byte3(regist);
    *((uint8_t *) memory + (*marx++)) = (uint8_t) byte2(regist);
    *((uint8_t *) memory + *marx) = (uint8_t) byte1(regist);
    return;
}


/*
   pull

   (Can) pull a full register from memory, starting
   from the most significant byte.

 */
void pull(uint32_t *mbrx, uint32_t *marx, void *memory)
{


    *mbrx = *((uint8_t *) memory + (*marx)++);
    *mbrx = (*mbrx << aByte);

    *mbrx |= *((uint8_t *) memory + (*marx)++);
    *mbrx = (*mbrx << aByte);

    *mbrx |= *((uint8_t *) memory + (*marx)++);
    *mbrx = (*mbrx << aByte);

    *mbrx |= *((uint8_t *) memory + (*marx)++);

}

//SCZ -> Checks for SIGN, CARRY, and ZERO flags


void SCZ(uint32_t op1, uint32_t op2)
{

    //SIGN
    if(isSIGN(ALU))
        set(SIGNF, &ccr);
    else
        clear(SIGNF, &ccr);

    //CARRY
    if(iscarry(op1, op2, isCARRYset(ccr)))
        set(CARRYF, &ccr);
    else
        clear(CARRYF, &ccr);

    //ZERO

    if(isZERO(ALU))
        set(ZEROF, &ccr);
    else
        clear(ZEROF, &ccr);

    return;
}

//SZ -> Just checks for SIGN and ZERO flags
//Stream-lines the process, basically.

void SZ()
{


    //SIGN
    if(isSIGN(ALU))
        set(SIGNF, &ccr);
    else
        clear(SIGNF, &ccr);

    //ZERO

    if(isZERO(ALU))
        set(ZEROF, &ccr);
    else
        clear(ZEROF, &ccr);

    return;
}



/*
   isREGmask

   Checks if it's a valid CCR mask
 */

uint8_t isREGMASK(uint16_t mask)
{
    if(mask == SIGNF)
        return SIGNF;

    if(mask == ZEROF)
        return ZEROF;

    if(mask == ((STOPF | IRF) | CARRYF))
        return ((STOPF | IRF) | CARRYF);

return 0;
}

//Toggles valid flag (mask)

void toggle(uint16_t mask, uint32_t *control)
{

    if(isREGMASK(mask))
        *control = ((*control & mask) == mask) ? (*control & ~mask) : (*control | mask);

    return;
}

//Sets the flag given

void set(uint16_t mask, uint32_t *control)
{

    if(isREGMASK(mask))
        *control |= mask;
    return;
}

//Clears the given flag (sets it to 0)

void clear(uint16_t mask, uint32_t *control)
{
    if(isREGMASK(mask))
        *control &= ~mask;

    return;
}

/**********************************************************
  iscarry()- determine if carry is generated by addition: op1+op2+C
  C can only have value of 1 or 0.
 ***********************************************************/
    int iscarry(unsigned long op1,unsigned long op2, unsigned C){
        if ((op2== MAX32)&&(C==1))
            return(1); // special case where op2 is at MAX32
        return((op1 > (MAX32 - op2 - C))?1:0);

    }

//Quite literally holds the display until enter should be pressed
void holdDisplay ( void )
{             //If function called
    int hold = 0;  //Holds screen until enter is specifically pressed
    while(hold = getchar() != '\n' && hold != EOF );
}
