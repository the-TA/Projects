/*
ISAAC HAAS
*/
#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>

using namespace std;
char ASM_FILE_NAME[] = "QuickSort.asm";

const int MAX = 1200;   //size of simulators memory
const int COL = 7;		//number of columns for output

	//	REGISTERS
const int AXREG = 0;
const int BXREG = 1;
const int CXREG = 2;
const int DXREG = 3;

	// COMMANDS
const int EXT = 1;		// Allows us to use a different set of commands
const int FUN = 3;			// function call
const int RET = 4;			// return
const int HALT = 5;			// halt
const int GET = 6;			// input into AX
const int PUT = 7;			// output from AX

const int OR = 32;			// logical OR
const int AND = 64;			// logical AND
const int CMP = 96;			// compare: set the flag
const int SUB = 128;		   // subtract
const int ADD = 160;		   // add
const int MOVREG = 192;		// move into register
const int MOVMEM = 224;		// move into memory

const int JE = 0;			   // jump if equal
const int JNE = 1;			// jump if not equal
const int JB = 2;		      // jump if below
const int JBE = 3;			// jump if below or equal
const int JA = 4;			   // jump if above
const int JAE = 5;			// jump if above or equal
const int JMP = 6;			// force jump (always)


	// OPERANDS
const int ARRAY_DREF = 4;     // [bx]	
const int STRUCT_DREF = 5;    //	[bx+xxxx]
const int MEM = 6;            // [xxxx]
const int CONST = 7;          // xxxx


enum paramType { reg, mem, constant, arrayBx, arrayBxPlus, none };
enum registerEnum { A, B, C, D, REG_NOT_DEFINED = -1 }; // REGISTERS

typedef short Memory;

class Registers
{
public:
	int AX;
	int BX;
	int CX;
	int DX;
	int funAdd;	// function pointer 
	int flag;	// 1: a>b	0: a=b	-1: a<b
	bool extend;
} regis; // regis: used to mimick the registers inside the computer. 

Memory memory[MAX] = { 0 };
int address;	// main program pointer
int stackAdd;	// stack pointer


void printMemoryDump(); //prints memory with integers commands
void convertToNumber(string line, int &start, int &value);	// Vicki's converts a string to an int
int convertToNumber(string toConvert);	// Isaac's: converts a string to an int
registerEnum whichReg(char regLetter);  //returns the enum of the letter register
void fillMemory();	// opens the file of assembly, changes the code to machine code and stores in memory
void runCode();	// runs the assembled machine code
void convertToMachineCode(ifstream &fin);	// reads in a line of assembly, converts to machine code, and stores it in memory
void changeToLowerCase(string &line);	// changes an entire string of chars to their lowercase equivalents
void putIntoReg(int value, int theRegNum);	// places an int value into the specified register
int processOperand(string operand);	// changes the operand from string to an int code
void splitCommand(string line, string &command, string &oper1, string &oper2);	// splits the line of assembly code into the 3 parts
int retrieveFromReg(int theRegNum);	// returns the value stored in the specified register
void thePush(int returnAddress);	// Stores the states. Called when fun is executed
void thePop();	// Retrieves the previous states. Called when ret is executed
void cleanLine(string &line); // gets the line of assembly ready to be split.