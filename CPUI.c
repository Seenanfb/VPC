/*
        Name: Seenan Bunni
        Program: This program is essentially a virtual CPU with a terminal interface.
        As well, it integrates some assembly language functions inside such as
        determining the instruction type of assembly language code.

*/

//Main includes

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#include "prototypes.h"
#include "define.h"
#include "registers.h"

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
    int read;
    unsigned offset, length;

    fprintf(stdout,"Seenan Bunni - 821 622 107\n\n");

    PC = 0;     //PC shouldn't be any less or more than 0. Seriously.

    for(;;){
        choice=menuChoice();
        if(choice == 'd' || choice == 'D'){
            fprintf(stdout,"Enter offset: ");
            fscanf(stdin, "%X", &offset);
            fprintf(stdout,"\nEnter length: ");
            fscanf(stdin, "%X", &length);
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
            fscanf(stdin, "%X", &offset);
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
int loadFile(void *memory, unsigned int max)
{


    char buff[150];
    memset(buff, 0, sizeof buff);
    FILE *thefile;
    int readin=0;
    struct stat stats;
    int filemax=0;
    int status;
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
    memset(fname, 0, sizeof fname);
    memset(buff, 0, sizeof buff);
    int bytes=0;
    FILE *f;
    int writein=0;
    char sizecheckcmd[50] = "ls -s ";
    FILE *p;
    char pbuff[100];

    fprintf(stdout,"Enter file name...\n");
    fscanf(stdin, "%[^\n]", fname);

    fprintf(stdout,"How many bytes should be written to file?\n");
    scanf("%d", &bytes); //scanf works fine/easier with digits, in my honest opinion
    while(bytes > MAXSIZE)
    {
        fprintf(stdout,"Error: Enter amount lower than or equal to %d\n", MAXSIZE);
        scanf("%d", &bytes);
    }

    f = fopen(fname, "wb+");
    writein = fwrite(memory, (size_t) 1, bytes, f);
}

/*
        memDump

        Dumps the memory data from the offset,
        to a specified length

*/

void memDump(void *memptr, unsigned offset, unsigned length)
{
    //isprint
    char *ptr;
    ptr = memptr;
    int i, j;
    int lengthOfRow = sixteen;

    for(i = offset; i < (offset + length); i+= lengthOfRow)
    {
        fprintf(stdout,"%04X\t", i);

        for(j = i; j < (i + lengthOfRow); j++)
        {
            fprintf(stdout,"%02X  ", (unsigned char) *(ptr + j));
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


void memMod(void *memory, unsigned offset)
{

    char buff[123];

    char *ptr;
    ptr = memory;

    memset(buff, 0, sizeof buff);
    buff[strlen(buff)] = '\0';
    int temp;
    int i, j;
    int lengthOfRow = sixteen;
    char *tempchar;
    int max = 0xFF;

    if(offset >= MAXSIZE)
        offset=0;

    fprintf(stdout,"Enter starting address (enter . to exit): \n");
    holdDisplay();
    while(1)
    {
        fprintf(stdout,"\n");
        fprintf(stdout,"%04X %X>  ", offset, *((unsigned char*) memory+offset) );
        fgets(buff, sizeof buff, stdin);

        if(buff[0] == '.')
            break;

        fprintf(stdout,"\n");
        if(sscanf(buff, "%X", &temp) == 0)
            perror("Non-hex digit found");
        else{
            if(temp > max){
                fprintf(stdout,"Out of range...\n");
            }
            else
                *((unsigned char*)memory + offset) = (unsigned char) temp;
            if((offset+=1) == MAXSIZE)
                return;
        }
        offset++;
    }

    fprintf(stdout,"\n");
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
            fprintf(stdout,"R%02d:  0x%08lX ", (i), r[i]);
            i++;
            if(i == 13)
                break;

            if(i % 4 == 0)
                fprintf(stdout,"\n");
        }

        if(ccr & 4)
            tempsign==1;
        else
            tempsign==0;
        if(ccr & 2)
            tempzero==1;
        else
            tempzero=0;
        if(ccr & 1)
            tempcarry==1;
        else
            tempcarry=0;

        fprintf(stdout,"\n");
        fprintf(stdout,"\nSP: 0x%08lX \tCCR: %d%d%d (SCZ)  MBR: 0x%08lX  MAR: 0x%08lX\n", SP, tempsign, tempzero, tempcarry, mbr, mar);
        fprintf(stdout,"LR: 0x%08lX \tIR: 0x%08lX  IR0: 0x%04lX      IR1: 0x%04lX\n", LR, ir, IR(ir), IR1(ir));
        fprintf(stdout,"PC: 0x%08lX\n", PC);

        fprintf(stdout,"\n");

        fprintf(stdout,"Stop Flag: %d\n", isSTOPset(stopflag));
        fprintf(stdout,"Active IR Flag: %d\n", IRoneORtwo(irflag));


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

        tempsign = 0;
        tempzero = 0;
        tempcarry = 0;

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

        //rev 2

        mbr=0;
        mar=PC;
        PC+=4;

        mbr+=*((unsigned char *) memory + (mar++));
        mbr=(mbr << aByte);

        mbr+=*((unsigned char *) memory + (mar++));
        mbr=(mbr << aByte);

        mbr+=*((unsigned char *) memory + (mar++));
        mbr=(mbr << aByte);

        mbr+=*((unsigned char *) memory + (mar++));

        ir=mbr;

        return;
    }
/*
        trace

        Traces through the program code, one line at a time
*/
    void trace(void *memory, int irflagx, unsigned long irx)
    {

        unsigned long irnum  = IR1(irx);
        unsigned long irnum1 = IR(irx);

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
            set(STOPF, (unsigned long *) &stopflag);
        return;
        }

        toggle(IRF, (unsigned long *) &irflag);


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
        while(!stopflag){
            if(irflag){
                execute(IR1(ir), memory);
                continue;
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
        immediate, it's sent here perform the
        necessary immediate operations for the
        instruction.


*/

    void immediate(uint16_t instr)
    {
        uint16_t op = IMMgetopcode(instr);
        uint8_t rd = RD(instr);
        uint16_t imm = IMMED(instr);

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


    void data(unsigned int instr)
    {

        unsigned long i;
        unsigned int rd = RD(instr);
        unsigned int rn = RN(instr);
        unsigned int op = dataGETOP(instr);

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


    }


/*
        loadstore

        Performs the necessary load or store
        function, if the instruction type
        is deemed to be a load or store



*/

    void loadstore(unsigned int instr, void *memory)
    {
        unsigned int rn = RN(instr);
        unsigned int rd = RD(instr);

        //LOAD

        if(LSload(instr))
        {
            if(LSdword(instr))
                pull(&ALU, ((unsigned long *) &rn), memory);
            else
                ALU = *((unsigned int *) memory + rn);

            r[rd] = ALU;
        }

        //STORE

        else
        {
            if(LSdword(instr)){
                mbr = r[rd];
                push(mbr, ((unsigned long *) &rn), memory);
            }
            else
                *((unsigned int *) memory + rn) = (unsigned int) r[rd] & aByteMASK;

        }
    }

/*
        pushpull

        This function handles the push and pull instruction.
        It will store or load register values from memory
        if need be.


*/

    void pushpull(unsigned int instr, void *memory)
    {

        unsigned int i, j;
        unsigned int maskset[] = {
            regmask1, regmask2, regmask3,
            regmask4, regmask5, regmask6,
            regmask7, regmask8 };

        unsigned int REGset = PPGETreg(instr);


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



    void condbr(unsigned int instr)
    {


        int addr = CONDGETaddr(instr);

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

void uncondbr(unsigned int instr, void *memory)
{


        unsigned int addr = UNBRGEToff(instr);

        mar=addr;

        if(isUNBRA(instr)){
                PC = (unsigned long) addr;
        return;
        }

        if(isUNBRL(instr)){
                push(LR, &mar, memory);
                PC = (unsigned long) addr;
        return;
        }
return;
}

/*
        push


        Pushes a full register into memory.

*/

void push(unsigned long regist, unsigned long *marx, void *memory)
{
    *((unsigned int *) memory + (*marx++)) = byte1(regist);
    *((unsigned int *) memory + (*marx++)) = byte2(regist);
    *((unsigned int *) memory + (*marx++)) = byte3(regist);
    *((unsigned int *) memory + *marx++) = byte4(regist);
    return;
}


/*

        pull

        (Can) pull a full register from memory, starting
        from the most significant byte.


*/


void pull(unsigned long *mbrx, unsigned long *marx, void *memory)
{

    *mbrx = *((unsigned int *) memory + (*marx)++);
    *mbrx = (*mbrx << aByte);

    *mbrx |= *((unsigned int *) memory + (*marx)++);
    *mbrx = (*mbrx << aByte);

    *mbrx |= *((unsigned int *) memory + (*marx)++);
    *mbrx = (*mbrx << aByte);

    *mbrx |= *((unsigned int *) memory + (*marx)++);

}

//SCZ -> Checks for SIGN, CARRY, and ZERO flags


void SCZ(unsigned int op1, unsigned int op2)
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

unsigned int isREGMASK(unsigned int mask)
{
    if(mask == SIGNF)
        return SIGNF;

    if(mask == ZEROF)
        return ZEROF;

    if(mask == ((STOPF | IRF) | CARRYF))
        return ((STOPF | IRF) | CARRYF);

}

//Toggles valid flag (mask)

void toggle(unsigned int mask, unsigned long *control)
{

    if(isREGMASK(mask))
        *control = ((*control & mask) == mask) ? (*control & ~mask) : (*control | mask);

    return;
}

//Sets the flag given

void set(unsigned int mask, unsigned long *control)
{

    if(isREGMASK(mask))
        *control |= mask;
    return;
}

//Clears the given flag (sets it to 0)

void clear(unsigned int mask, unsigned long *control)
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
