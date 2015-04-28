/*
Name: Seenan Bunni
registers.h

Header file that holds all the register
declarations for the CPU.

 */


#include <stdint.h> //Needed to declare uintX_t types

//All Registers, including the IR/Stop Flags

uint32_t r[16];
uint32_t mbr;
uint32_t mar;
uint32_t ALU;
uint32_t ir;
uint32_t ccr;

uint8_t irflag = 0;
uint8_t stopflag = 0;

#define SP r[13] //Stack Pointer definition
#define LR r[14] //Link Register definition
#define PC r[15] //Program Counter definition
