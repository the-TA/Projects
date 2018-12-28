/*
AUTHOR:			Isaac
INSTRUCTOR:		Victoria Eisele
TITLE:			CSC 225 Final Project
DUE:			12/12/2017
DESCRIPTION:	Reads in a file containing assembly code and interprets it, storing the corresponding values in a simmulated memory.
It then simmulates running the assembled machine code.
The assembly code is a program that sorts an array of numbers using either Bubble sort or Quick sort.
The assembly language used is a simplified form of x86
We used the ; to denote comments in the asm file

INPUT FILE: QuickSort.asm
*/

#include"Assembler.h"

int main()
{

	address = 0;
	stackAdd = MAX - 2;


	fillMemory();
	printMemoryDump();

	runCode();

	system("pause");
	return 0;
}


/* ________________________________________________________________________________________________________
-------------------fillMemory-------------------
opens the file of assembly, changes it to machine code and places the converted commands into memory. 
__________________________________________________________________________________________________________*/
void fillMemory()
{
	address = 0;
	ifstream fin;	// file pointer

	fin.open(ASM_FILE_NAME);
	if (fin.fail())
	{
		cout << "Error, file didn't open\n";
		system("pause");
		exit(1);
	}

	for (int i = 0; i < MAX && !fin.eof(); i++)
	{
		convertToMachineCode(fin);
	}
}

/* ________________________________________________________________________________________________________
-------------------whichReg-------------------
	changes the letter of the register to a number( registerEnum ).		
__________________________________________________________________________________________________________*/
registerEnum whichReg(char regLetter)
{
	if (regLetter == 'a')
	{
		return A;
	}
	else if (regLetter == 'b')
	{
		return B;
	}
	else if (regLetter == 'c')
	{
		return C;
	}
	else if (regLetter == 'd')
	{
		return D;
	}
	return REG_NOT_DEFINED;  //something went wrong if this is returned: not a register
}

/* ________________________________________________________________________________________________________
-------------------convertToNumber-------------------
  takes in a line and converts digits to a integer
  line - is the string of assembly code to convert
  start - is the location where the line is being converted,
it starts at the beginning of number and it passed
back at the next location 
  value - is the integer value of the digits in the code
__________________________________________________________________________________________________________*/
void convertToNumber(string line, int &start, int &value)
{
	char number[16];
	bool negative = false;
	int i = 0;
	if (line[start] == '-')
	{
		start++;
		negative = true;
	}
	while (i < 16 && line.size() > start && isdigit(line[start]))
	{
		number[i] = line[start];
		i++;
		start++;
	}
	number[i] = '\0';
	value = atoi(number);
	if (negative == true)
	{
		value = -value;
	}
}

/* ________________________________________________________________________________________________________
------------------- convertToNumber   OVERLOAD -------------------
This is our version. Converts a string to an int by looking at each character,
figuring out the literal value of the digit, and then multiplying it by 10 to the power of the location
ex: 358 = [(3 * 10^2) + (5 * 10^1) + (8 * 10^0)]
Returns the value of the converted int
________________________________________________________________________________________________________*/
int convertToNumber(string toConvert)
{
	int size = toConvert.length();	// the number of characters
	int power = size - 1;	// this is the exponent for the base, which is how we convert
	int constantValue = 0;	// the value of the string to return
	for (int k = 0; k < size; k++)
	{
		constantValue += (toConvert[k] - 48) * pow(10, power);
		power--;
	}
	return constantValue;
}

/* ________________________________________________________________________________________________________
------------------- printMemoryCommands ------------------- 
 prints memory with letter commands
 ________________________________________________________________________________________________________*/
void printMemoryCommands()
{
	int i = 0;
	int count;  // which column is currently outputting
	int moveColumn = MAX / COL;
	int index = 0;
	while (i< MAX)
	{
		count = 0;
		while (count < COL && index < MAX)
		{
			index = i + count * moveColumn;
			//As each type of instruction is added you will need to be able to deal with type of command
			//printcommand(index)
		}
		cout << endl;

	}
	cout << "RA " << regis.AX << "  BX " << regis.BX << "  CX " << regis.CX << "  DX " << regis.DX << endl;
	cout << "\tfunAdd " << regis.funAdd << "\tflag " << regis.flag << "\n\n";
}

/* ________________________________________________________________________________________________________
------------------- printMemoryDump ------------------- 
 prints memory by number
 MAX is the amount of elements in the memory array (Vicki used 100)
 COL is the number of columns that are to be displayed (Vicki used 7; was originally called COLUMNS) 
 ________________________________________________________________________________________________________*/
void printMemoryDump()
{
	int numRows = MAX / COL + 1;   //number of rows that will print
	int carryOver = MAX%COL;   //number of columns on the bottom row
	int location;   //the current location being called
	for (int row = 0; row<numRows; row++)
	{
		location = row;
		for (int column = 0; location<MAX&&column<COL; column++)
		{
			if (!(numRows - 1 == row&&carryOver - 1<column))
			{
				cout << setw(5) << location << "." << setw(5) << memory[location];
				location += (numRows - (carryOver - 1<column));
			}
		}
		cout << endl;
		//cout<<setw(3);
	}

	cout << endl;
	cout << "Address: " << address << endl;
	cout << "AX: " << regis.AX << '\t';
	cout << "BX: " << regis.BX << '\t';
	cout << "CX: " << regis.CX << '\t';
	cout << "DX: " << regis.DX << '\t';
	cout << endl << endl;
	cout << "Function Address: " << regis.funAdd << endl;
	cout << "Stack Address: " << stackAdd << endl;
	cout << "Flag: " << regis.flag;

	cout << endl << endl;
}

/* ________________________________________________________________________________________________________
------------------- changeToLowerCase ------------------- 
 iterates through a string and individually converts each to lowercase
 LINE: the number in string format
 __________________________________________________________________________________________________________*/
void changeToLowerCase(string &line)
{
	int index = 0;
	while (index < line.size())
	{
		line[index] = tolower(line[index]);
		index++;
	}
}

/* ________________________________________________________________________________________________________
------------------- runCode ------------------- 
reads the machine code from memory and executes the instructions in C++ code,
simulating the computers interpretation of the machine code
__________________________________________________________________________________________________________*/
void runCode()
{
	Memory command;	// an entire line of assembly instruction in machine code
	Memory topBits, midBits, bottomBits;	// the command split into the 3 major components: command, reg, and mmm
	Memory reg;	// The register where the value will end up in

	address = 0;
	while (memory[address] != HALT)              //runs until command halt which equals 5
	{
		command = memory[address];
		address++;
		topBits = command & 224;              // these bit comparisons determine what bits are on for instruction processing
		midBits = command & 24;
		bottomBits = command & 7;

		if (regis.extend)
		{
			regis.extend = false;
			if (topBits == 0)
			{
				if (midBits == 0)
				{
					if (bottomBits == PUT) //putChar
					{
						cout << char(regis.AX);
					}
				}
			}
		}
		else
		{
			if (topBits == MOVREG)
			{
				reg = midBits >> 3;                 // midBits are shifted to right to get register type
				if (bottomBits < 4) // it is a register
				{
					putIntoReg(retrieveFromReg(bottomBits), reg);
				}
				else if (bottomBits == ARRAY_DREF)	//[BX]
				{
					putIntoReg(memory[regis.BX], reg);
				}
				else if (bottomBits == STRUCT_DREF)
				{
					putIntoReg(memory[regis.BX + memory[address]], reg);
					address++;
				}
				else if (bottomBits == MEM)	// a memory location
				{
					putIntoReg(memory[memory[address]], reg); // memory[address] is another memory address, which references the value to be moved into the reg
					address++;
				}
				else if (bottomBits == CONST)    // oper2 is a constant
				{
					putIntoReg(memory[address], reg);
					address++;
				}
			}
			else if (topBits == ADD)
			{
				reg = (midBits >> 3);
				int sum = retrieveFromReg(reg);
				if (bottomBits == CONST) // is a const
				{
					sum += memory[address];
					address++;
				}
				else if (bottomBits == MEM)
				{
					sum += memory[memory[address]];
					address++;
				}
				else if (bottomBits == STRUCT_DREF)
				{
					sum += memory[regis.BX + memory[address]];
					address++;
				}
				else if (bottomBits == ARRAY_DREF)
				{
					sum += memory[regis.BX];
				}
				else // bottomBits is a reg
				{
					sum += retrieveFromReg(bottomBits);
				}
				putIntoReg(sum, reg);
			}
			else if (topBits == SUB)
			{
				reg = (midBits >> 3);
				int differance = retrieveFromReg(reg);
				if (bottomBits == CONST) // is a const
				{
					differance -= memory[address];
					address++;
				}
				else if (bottomBits == ARRAY_DREF)
				{
					differance -= memory[regis.BX];
				}
				else // bottomBits is a reg
				{
					differance -= retrieveFromReg(bottomBits);
				}
				putIntoReg(differance, reg);
			}
			else if (topBits == AND)
			{
				reg = (midBits >> 3);
				int result = retrieveFromReg(reg);
				if (bottomBits == CONST) // is a const
				{
					result = (result & memory[address]);
					address++;
				}
				else // bottomBits is a reg
				{
					result = (result & retrieveFromReg(bottomBits));
				}
				putIntoReg(result, reg);
			}
			else if (topBits == OR)
			{
				reg = (midBits >> 3);
				int result = retrieveFromReg(reg);
				if (bottomBits == CONST) // is a const
				{
					result = (result | memory[address]);
					address++;
				}
				else // bottomBits is a reg
				{
					result = (result | retrieveFromReg(bottomBits));
				}
				putIntoReg(result, reg);
			}
			else if (topBits == MOVMEM)
			{
				reg = (midBits >> 3);
				if (bottomBits == ARRAY_DREF)	// [BX]
				{
					memory[regis.BX] = retrieveFromReg(reg);
				}
				else if (bottomBits == STRUCT_DREF)
				{
					memory[regis.BX + memory[address]] = retrieveFromReg(reg);
					address++;
				}
				else
				{
					memory[memory[address]] = retrieveFromReg(reg);
					address++;
				}
			}
			else if (topBits == CMP)
			{
				reg = (midBits >> 3);
				int aTerm, bTerm;	// these are for the compare.
									//	if aTerm > bTerm:	1
									//	if aTerm == bTerm:	0
									//	if aTerm < bTerm:	-1

				aTerm = retrieveFromReg(reg);

				if (bottomBits == CONST) // bottomBits is a const
				{
					bTerm = memory[address];
					address++;
				}
				else if (bottomBits == 6) // mem
				{
					bTerm = memory[memory[address]];
					address++;
				}
				else if (bottomBits == STRUCT_DREF)
				{
					bTerm = memory[regis.BX + memory[address]];
					address++;
				}
				else if (bottomBits == ARRAY_DREF)
				{
					bTerm = memory[regis.BX];
				}
				else // bottomBits is a reg
				{
					bTerm = retrieveFromReg(bottomBits);
				}
				regis.flag = (aTerm > bTerm) ? 1 : (aTerm == bTerm) ? 0 : -1;
			}
			else if (topBits == 0)
			{
				if (midBits == 0)
				{
					if (bottomBits == GET)
					{

						cout << "\ninput into AX: ";
						cin >> regis.AX;
					}
					else if (bottomBits == PUT)
					{
						cout << "\nAX is " << regis.AX << endl << endl;
					}
					else if (bottomBits == FUN) // Function ******************************
					{

						int numParams;
						regis.funAdd = memory[address];
						address++;
						numParams = memory[address];
						address++;
						for (int i = 1; i <= numParams; i++)
						{
							if (memory[address] < 4)
							{
								memory[regis.funAdd - i] = retrieveFromReg(memory[address]);
								address++;
							}
							else if (memory[address] == MEM)
							{
								address++;
								memory[regis.funAdd - i] = memory[memory[address]];
								address++;
							}
							else if (memory[address] == CONST)
							{
								address++;
								memory[regis.funAdd - i] = memory[address];
								address++;
							}
						}
						thePush(address);
						//cout << "\nThe return address is " << address << endl << endl;
						address = regis.funAdd;
					}
					else if (bottomBits == RET)
					{
						memory[memory[address]] = regis.AX;
						thePop();
					}
					else if (bottomBits == EXT)
					{
						regis.extend = true;
					}
				}
				else if (midBits == 8) // this is 000 01 iii for the jump commands
				{
					switch (bottomBits)
					{
					case JE:
						if (regis.flag == 0)
						{
							address = memory[address];
						}
						else
						{
							address++;
						}
						break;
					case JNE:
						if (regis.flag != 0)
						{
							address = memory[address];
						}
						else
						{
							address++;
						}
						break;
					case JB:
						if (regis.flag == -1)
						{
							address = memory[address];
						}
						else
						{
							address++;
						}
						break;
					case JBE:
						if (regis.flag != 1)
						{
							address = memory[address];
						}
						else
						{
							address++;
						}
						break;
					case JA:
						if (regis.flag == 1)
						{
							address = memory[address];
						}
						else
						{
							address++;
						}
						break;
					case JAE:
						if (regis.flag != -1)
						{
							address = memory[address];
						}
						else
						{
							address++;
						}
						break;
					case JMP:
						address = memory[address];
						break;
					}
				}
			}
		}
	/*	printMemoryDump();
		system("pause");*/
	}
}

/*
------------------- putIntoReg ------------------- 
places an int value into the specified register
return: void
parameters: the constant to put into the register
the register itself
__________________________________________________________________________________________________________*/
void putIntoReg(int value, int theRegNum)
{
	switch (theRegNum)
	{
	case A:
		regis.AX = value;
		break;
	case B:
		regis.BX = value;
		break;
	case C:
		regis.CX = value;
		break;
	case D:
		regis.DX = value;
		break;
	default:
		break;
	}
}

/* ________________________________________________________________________________________________________
------------------- processOperand ------------------- 
changes the operand from string to an int code
return: the code that corresponds to the operand
parameters: the operand to be converted
__________________________________________________________________________________________________________*/
int processOperand(string operand)
{
	changeToLowerCase(operand);
	int machineCode = whichReg(operand[0]);
	if (machineCode == REG_NOT_DEFINED)
	{
		if (operand[0] == '[')
		{
			if (operand.find('+') != string::npos) // [xxxx+BX]
			{
				machineCode = STRUCT_DREF;
			}
			else if (operand.find('b') != string::npos) // [BX]
			{
				machineCode = ARRAY_DREF;
			}
			else // this is just a memory location: [xxxx]
			{
				machineCode = MEM;
			}
		}
		else // const
		{
			machineCode = CONST;
		}
	}
	return machineCode;
}

/* ________________________________________________________________________________________________________
------------------- splitCommand ------------------- 
this breaks up the command line into the 3 compnents: command, oper1, and oper2
return: void
parameters: line: the assembly code string read in from the file.
command: the first portion of the line. The assembly command, literally
oper1: the first operand
oper2: the second operand
__________________________________________________________________________________________________________*/
void splitCommand(string line, string &command, string &oper1, string &oper2)
{
   cleanLine(line);

	int spaceLoc = line.find(' ');

	if (spaceLoc != string::npos)	// string::npos is what string.find() returns if char not found
	{
		command = line.substr(0, spaceLoc);
		line = line.substr(spaceLoc + 1);
		spaceLoc = line.find(' ');
		if (spaceLoc != string::npos)
		{
			oper1 = line.substr(0, spaceLoc);
			line = line.substr(spaceLoc + 1);
			spaceLoc = line.find(' ');
			oper2 = line.substr(0, spaceLoc);
		}
		else
		{
			oper1 = line;
		}
	}
	else
	{
		command = line;
	}
	
}

/* ________________________________________________________________________________________________________
------------------- convertToMachineCode ------------------- 
This ASSEMBLES the assembly code and then stores it in memory
return: true until the HALT command is read
parameters: fin: the file containing the assembly code to interpret
__________________________________________________________________________________________________________*/
void convertToMachineCode(ifstream &fin)
{
	string line, command, oper1, oper2;	// the 3 parts of every line of assembly (besides halt) 
	int machineCode;					// the binary representation of the assembly commands
	bool keepGoing = true;				// this is for the calling function. It is set to false when the HALT command is read.

	getline(fin, line, '\n');

	splitCommand(line, command, oper1, oper2);	// this breaks up the assembly code lines into 3 pieces so that we can interpret them

	if (command.empty())	// this is an empty line in the asm file	
	{
		address++;
	}
	else if (command[0] == 'h')   // HALT	
	{
		memory[address] = HALT;
		address++;
		keepGoing = false;
	}
	else if (command[0] == 'e')
   {
		memory[address] = EXT;
		address++;
   }
	else if (command[0] == 'g')	// GET
	{
		memory[address] = GET;
		address++;
	}
	else if (command[0] == 'p')	// PUT
	{
		memory[address] = PUT;
		address++;
	}
	else if (command[0] == 'j')	// one of the jump commands
	{
		if (command[1] == 'e')
		{
			machineCode = 8 + JE;
		}
		else if (command[1] == 'n')
		{
			machineCode = 8 + JNE;
		}
		else if (command[1] == 'b')
		{
			if (command.length() == 2)
			{
				machineCode = 8 + JB;
			}
			else
			{
				machineCode = 8 + JBE;
			}
		}
		else if (command[1] == 'a')
		{
			if (command.length() == 2)
			{
				machineCode = 8 + JA;
			}
			else
			{
				machineCode = 8 + JAE;
			}
		}
		else if (command[1] == 'm')
		{
			machineCode = 8 + JMP;
		}
		memory[address] = machineCode;
		address++;
		memory[address] = convertToNumber(oper1.substr(1, oper1.length() - 2));	// oper1 is formatted [xxxx] so the substr(1, length - 2) illiminates the [] from the address
		address++;
	}
	else if (isdigit(command[0])) // just a number
	{
		memory[address] = convertToNumber(command);
		address++;
	}
	else if (command[0] == 'r')   // return
	{
		memory[address] = RET;
		address++;
		memory[address] = convertToNumber(oper1.substr(1, oper1.length() - 2));
		address++;
	}
	else if (command[0] == 'f')   // function
	{
		int numParameters = convertToNumber(oper2);
		int funAddress = convertToNumber(oper1.substr(1, oper1.length() - 2));
		int convertedVal;

		memory[address] = FUN;
		address++;
		memory[address] = funAddress;
		address++;
		memory[address] = numParameters;
		address++;

		for (int i = 0; i < numParameters; i++)
		{
			getline(fin, line, '\n');
         cleanLine(line);

			if ( whichReg( line[0] ) != REG_NOT_DEFINED )
			{
				convertedVal = whichReg(line[0]);
				memory[address] = convertedVal;
				address++;
			}
			else if (line[0] == '[')
			{
				if (line.find('+' != string::npos))
				{
					memory[address] = STRUCT_DREF;

				}
				memory[address] = MEM;
				address++;
				convertedVal = convertToNumber(line.substr(1, line.length() - 2));
				memory[address] = convertedVal;
				address++;
			}
			else if (isdigit(line[0]))
			{
				memory[address] = CONST;
				address++;
				convertedVal = convertToNumber(line);
				memory[address] = convertedVal;
				address++;
			}
		}
	}
	
	
	else // this is the commands with three operands
	{

		if (command[0] == 'm')
		{
			if (oper1[0] == '[')		// mov mem
			{
				machineCode = MOVMEM;
				oper2.swap(oper1);
			}
			else // movreg
			{
				machineCode = MOVREG;
			}
		}
		else if (command[1] == 'd')		// ADD (checking the second char to distinguish between AND)
		{
			machineCode = ADD;
		}
		else if (command[0] == 's')		// SUBTRACT
		{
			machineCode = SUB;
		}
		else if (command[0] == 'o')		// OR
		{
			machineCode = OR;
		}
		else if (command[1] == 'n')		// AND (checking the second char to distinguish between ADD)
		{
			machineCode = AND;
		}
		else if (command[0] == 'c')		// CMP (compare)
		{
			machineCode = CMP;
		}

		machineCode += (processOperand(oper1) << 3) + processOperand(oper2);
		memory[address] = machineCode;
		address++;

		if (processOperand(oper2) > 4)  // requires an extra line
		{
			if (oper2.find('+') != string::npos)	// [xxx+BX]
			{
				if ( oper2[1] == 'b' )
					machineCode = convertToNumber(oper2.substr(oper2.find('+') + 1, oper2.length() - oper2.find('+') - 2));
				else
					machineCode = convertToNumber(oper2.substr(1, oper2.find('+') - 1));
			}
			else if (oper2[0] == '[')	// [xxx]
			{
				machineCode = convertToNumber(oper2.substr(1, oper2.length() - 2));
			}
			else	// const
			{
				machineCode = convertToNumber(oper2);
			}
			memory[address] = machineCode;
			address++;
		}
	}
}

/* ________________________________________________________________________________________________________
------------------- retrieveFromReg ------------------- 
gets the value from a register
parameters: the number of the register you want to retrieve from
return: the value from the specified register
__________________________________________________________________________________________________________*/
int retrieveFromReg(int theRegNum)
{
	int returnVal;	// The value from the specified register, or 0 if none
	switch (theRegNum)
	{
	case A:
		returnVal = regis.AX;
		break;
	case B:
		returnVal = regis.BX;
		break;
	case C:
		returnVal = regis.CX;
		break;
	case D:
		returnVal = regis.DX;
		break;
	default:
		returnVal = 0;
		break;
	}
	return returnVal;
}

/* thePush
Stores the states on the stack
Return: void
Parameters: the address from which the function was called
*/
void thePush(int returnAddress)
{
	memory[stackAdd] = regis.AX;
	stackAdd--;
	memory[stackAdd] = regis.BX;
	stackAdd--;
	memory[stackAdd] = regis.CX;
	stackAdd--;
	memory[stackAdd] = regis.DX;
	stackAdd--;
	memory[stackAdd] = regis.flag;
	stackAdd--;
	memory[stackAdd] = returnAddress;
	stackAdd--;
}

/* thePop
Restores the states from the stack
Return: void
Parameters: none
*/
void thePop()
{
	stackAdd++;
	address = memory[stackAdd];
	stackAdd++;
	regis.flag = memory[stackAdd];
	stackAdd++;
	regis.DX = memory[stackAdd];
	stackAdd++;
	regis.CX = memory[stackAdd];
	stackAdd++;
	regis.BX = memory[stackAdd];
	stackAdd++;
	regis.AX = memory[stackAdd];
}

/*    cleanLine
Removes comments and spaces from the line of assembly.
Parameters: the line of assembly from the input file
Return: void
*/
void cleanLine(string &line)
{
   int spaceLoc;

   changeToLowerCase(line);

   if (line.find(';') != string::npos) // comment
   {
      line = line.substr(0, line.find(';'));
   }
   int i = line.length() - 1;
   while (i >= 0 && (line[i] == ' ' || line[i] == '\t'))	// delete all spaces from the end of the line
   {
      line = line.substr(0, i);
      i--;
   }
}


//Issues and problems: natta
