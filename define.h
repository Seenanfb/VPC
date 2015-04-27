
//Byte defines and Flags

#define aByte 8
#define aByteMASK 0xFF

#define SIGNF  4
#define ZEROF  2
#define CARRYF 1
#define STOPF  1
#define IRF    1


//Check Instruction type(?)

#define DATAinst(x)     ((instrMask & ~x) == DATAmask)  ? 1 : 0
#define LSinst(x)       ((instrMask & x) == LSmask)     ? 1 : 0
#define IMMinst(x)     (((instrMask & x) == IMMmask) || ((instrMask & x) == IMM2mask))    ? 1 : 0
#define CONDBRinst(x)   ((instrMask & x) == CONDBRmask) ? 1 : 0
#define PSHPLLinst(x)   ((instrMask & x) == PSHPLLmask) ? 1 : 0
#define UNBRinst(x)     ((instrMask & x) == UNBRmask)   ? 1 : 0
#define STOPinst(x)     ((instrMask & x) == STOPmask)   ? 1 : 0

//Masks

#define RDmask 0x000F
#define RNmask 0x00F0
#define Rshift 4

#define instrMask 0xE000

#define DATAmask   0xF000
#define LSmask     0x2000
#define IMMmask    0x4000
#define IMM2mask   0x6000
#define CONDBRmask 0x8000
#define PSHPLLmask 0xA000
#define UNBRmask   0xC000

#define STOPmask   0xE000

#define SIGNmask      0x80000000
#define MostSBmask    0x80000000
#define LowSBmask     1

#define IRmask  0xFFFF0000
#define IR1mask 0x0000FFFF
#define IR(x)   (x & IRmask) >> sixteen
#define IR1(x)  (x & IR1mask)

//Some register/instruction definitions

#define regsize   4
#define instrsize 2


//Other or Flag Checks

#define RN(x) ((x & RNmask) >> Rshift)
#define RD(x) (x & RDmask)

#define isSIGN(x) ((SIGNmask & x) == SIGNmask) ? 1 : 0
#define isZERO(x) (x==0) ? 1 : 0

#define isSIGNset(x)  (SIGNF & x)  ? 1 : 0
#define isZEROset(x)  (ZEROF & x)  ? 1 : 0
#define isCARRYset(x) (CARRYF & x) ? 1 : 0
#define isSTOPset(x)  (STOPF & x)  ? 1 : 0
#define IRoneORtwo(x) (IRF & x)    ? 1 : 0

#define hasMSB(x) ((MostSBmask & x) == MostSBmask) ? 1 : 0
#define hasLSB(x) ((LowSBmask & x) == LowSBmask) ? 1 : 0


//Data processing/OP codes

#define ANDcode 0x0000
#define EORcode 0x0100
#define SUBcode 0x0200
#define SXBcode 0x0300
#define ADDcode 0x0400
#define ADCcode 0x0500
#define LSRcode 0x0600
#define LSLcode 0x0700
#define TSTcode 0x0800
#define TEQcode 0x0900
#define CMPcode 0x0A00
#define RORcode 0x0B00
#define ORRcode 0x0C00
#define MOVcode 0x0D00
#define BICcode 0x0E00
#define MVNcode 0x0F00

#define dataOPcode 0x0F00
#define dataOPshift 8
#define dataGETOP(x) (dataOPcode & x) >> dataOPshift

//Immediate

#define IMMshift 4

#define IMMmov 0x4000
#define IMMcmp 0x5000
#define IMMadd 0x6000
#define IMMsub 0x7000

#define IMMval 0x0FF0

#define IMMED(x) ((x & IMMval) >> IMMshift)

#define IMMopcode 0x7000
#define IMMgetopcode(x) (IMMopcode & x)

//Load/Store

#define LSLmask 0x0800
#define LSBmask 0x0400

#define LSload(x)  ((LSLmask & x) == LSLmask) ? 1 : 0
#define LSdword(x) ((LSBmask & x) == LSBmask) ? 1 : 0

#define byte1(x) (0xFF & x)
#define byte2(x) ((0xFF00 & x) >> aByte)
#define byte3(x) ((0xFF0000 & x) >> aByte)
#define byte4(x) ((0xFF000000 & x) >> aByte)


//Push/Pull

#define LOWreg  7
#define HIGHreg 8

#define PPOPL 0x0800
#define PPOPH 0x0400
#define PPOPR 0x0100

#define PPopcode 0x0F00
#define PPGETop(x) (PPopcode & x)

#define PPreg 0x00FF
#define PPGETreg(x) (PPreg & x)

#define regmask1 0x01 //Masks for push/pull
#define regmask2 0x02
#define regmask3 0x04
#define regmask4 0x08
#define regmask5 0x10
#define regmask6 0x20
#define regmask7 0x40
#define regmask8 0x80

#define PPush(x)   (PPGETop(x) == PPOPL) ? 1 : 0
#define PPhigh(x)  (PPGETop(x) == PPOPH) ? 1 : 0
#define PPextra(x) (PPGETop(x) == PPOPR) ? 1 : 0


//Conditional Branch codes

#define CONDeq 0x8000
#define CONDne 0x8100
#define CONDcs 0x8200
#define CONDcc 0x8300
#define CONDmi 0x8400
#define CONDpl 0x8500

#define CONDhi 0x8800
#define CONDls 0x8900

#define CONDal 0x8E00

#define CONDopcode     0x0F00
#define CONDaddr       0x00FF

#define CONDGETop(x)   (CONDopcode & x)
#define CONDGETaddr(x) (CONDaddr & x)


//Unconditional Branch codes

#define UNBRbra 0xC000
#define UNBRbrl 0xD000

#define isUNBRA(x)  ((UNBRbra & x) == UNBRbra) ? 1 : 0
#define isUNBRL(x)  ((UNBRbrl & x) == UNBRbrl) ? 1 : 0

#define UNBRoffset    0x0FFF
#define UNBRGEToff(x) (UNBRoffset & x)
