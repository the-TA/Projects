/* Isaac Haas 
12/14/2015
Final Project CSC 119 601
Minesweeper game
*/

#include <iostream>
#include <iomanip>
#include <ctime>
#include <Windows.h>
using namespace std;

const int MAX = 9;				// The size of the column and rows
const int BOMBS = 10;			// Number of bombs
const int UNCHECKEDVAL = 9;	// Value within the array- Pre-game
const int BOMBVALUE = 10;		// Value of the bomb within the array
const int FLAGVALUE = -10;		// Value of the flags within the array
const int YELLOW = 14, PINK = 13, RED = 12, BLUE = 11, GREEN = 10;	// colors for printBoard

void startArray(int mineArray[MAX][MAX]);				// Fills the array and adds ten bombs
void printArray(int mineArray[MAX][MAX]);				// Prints the array.  Debugging.
int countNeighborBombs(int mineArray[MAX][MAX], int centerX, int centerY);		// Counts the bombs around the cell and user's flags
bool checkValidity(int x, int y);						// Makes sure the cells that it is checking is on the board
void showMyCount(int mineArray[MAX][MAX], int centerX, int centerY);		// The recurrsion
void printBoard(int mineArray[MAX][MAX]);				// The board, with colors, and smiley/dead faces!
bool gameOverCheck(int mineArray[MAX][MAX]);			// Checks if the game is over
int bombCount(int mineArray[MAX][MAX]);				// Gives the bomb count
void winSmile();												// Win outcome smile
void loseFrown();												// Lose outcome smile


int main()
{
	HANDLE hConsole;											// Color
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);		// Colors
	int mineArray[MAX][MAX];			// The 2D array for the board
	int userX, userY;						// User's choice
	bool cont = true;						// If user hits bomb = false and ends the game
	bool flag;								// Lets user flag bombs. 
	srand(time(NULL));					// Random number generator

	startArray(mineArray);
	printBoard(mineArray);
	
	do // the game
	{
		do	// validation
		{
			cout << "Pick a cell (Column/Row)" << endl;
			cin >> userX >> userY;
		} while (checkValidity(userX, userY) == false);

						// 
		cout << "You chose the cell at column " << userX << " and row " << userY << endl;
		cout << "Do you want to flag it or flip it? Enter 1 for flag, 0 for flip." << endl;
		cin >> flag;

		if (flag)
		{
			mineArray[userX][userY] *= -1;
			printBoard(mineArray);
		}
		else
		{
			if (mineArray[userX][userY] != BOMBVALUE)
			{
				showMyCount(mineArray, userX, userY);
				printBoard(mineArray);
			}
			else
			{
				cout << "You have chosen a bomb." << endl << "You loose." << endl;
				cont = false;
			}
			
		}



	} while (cont && gameOverCheck(mineArray));

	if (cont == false)	// If you blew up, print sad face
	{
		loseFrown();
	}
	else						// If you didn't blow up, print happy face
	{
		winSmile();
	}

	cout << endl;
	system("pause");
	return 0;
}

/* startArray
fills board with UNCHECKEDVAL, and then ten BOMBVALUEs
parameter: the board
*/
void startArray(int mineArray[MAX][MAX])
{
	int randNum1, randNum2;			// two random numbers, to generate random indeces.

	for (int x = 0; x < MAX; x++)
	{
		for (int y = 0; y < MAX; y++)
		{
			mineArray[x][y] = UNCHECKEDVAL;
		}
	}
	for (int i = 0; i < BOMBS; i++)
	{
		do
		{
			randNum1 = rand() % MAX;
			randNum2 = rand() % MAX;
		} while (mineArray[randNum1][randNum2] == BOMBVALUE);

		mineArray[randNum1][randNum2] = BOMBVALUE;
	}
}

/* printArray
Prints array to screen (for debugging purposes only)
parameter: the board
*/
void printArray(int mineArray[MAX][MAX])
{
	
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << setw(4) << mineArray[x][y];
		}
		cout << endl << endl;
	}
}

/* countNeighborBombs
counts the bombs in the eight surrounding cells
return: number of bombs 0-8
parameter: the cell and his eight neighbors
*/
int countNeighborBombs(int mineArray[MAX][MAX], int centerX, int centerY)
{
	int count = 0;	// Neighbor bomb count. (0-8)

	if (checkValidity(centerX - 1, centerY - 1) && (mineArray[centerX - 1][centerY - 1] == BOMBVALUE || mineArray[centerX - 1][centerY - 1] == FLAGVALUE))
	{
		count++;
	}
	if (checkValidity(centerX - 1, centerY) && (mineArray[centerX - 1][centerY] == BOMBVALUE || mineArray[centerX - 1][centerY] == FLAGVALUE))
	{
		count++;
	}
	if (checkValidity(centerX - 1, centerY + 1) && (mineArray[centerX - 1][centerY + 1] == BOMBVALUE || mineArray[centerX - 1][centerY + 1] == FLAGVALUE))
	{
		count++;
	}

	if (checkValidity(centerX, centerY - 1) && (mineArray[centerX][centerY - 1] == BOMBVALUE || mineArray[centerX][centerY - 1] == FLAGVALUE))
	{
		count++;
	}
	if (checkValidity(centerX, centerY + 1) && (mineArray[centerX][centerY + 1] == BOMBVALUE || mineArray[centerX][centerY + 1] == FLAGVALUE))
	{
		count++;
	}

	if (checkValidity(centerX + 1, centerY - 1) && (mineArray[centerX + 1][centerY - 1] == BOMBVALUE || mineArray[centerX + 1][centerY - 1] == FLAGVALUE))
	{
		count++;
	}
	if (checkValidity(centerX + 1, centerY) && (mineArray[centerX + 1][centerY] == BOMBVALUE || mineArray[centerX + 1][centerY] == FLAGVALUE))
	{
		count++;
	}
	if (checkValidity(centerX + 1, centerY + 1) && (mineArray[centerX + 1][centerY + 1] == BOMBVALUE || mineArray[centerX + 1][centerY + 1] == FLAGVALUE))
	{
		count++;
	}


	return count;
}

/* checkValidity
checks if cell is on board
parameter: cell to check
return: valid: (y/n)
*/
bool checkValidity(int x, int y)
{
	bool returnVal = false;	// for valid cell

	if (x >= 0 && x < MAX && y >= 0 && y < MAX)  // checks if cell is on board
	{
		returnVal = true;
	}
	return returnVal;
}

/* showMyCount
Recursive function
Checks if valid, if not bomb, flag or fliped, then counts bombs, then recursion
parameter: the cell, and the board
*/
void showMyCount(int mineArray[MAX][MAX], int centerX, int centerY)
{

	if (checkValidity(centerX, centerY))
	{
		if (mineArray[centerX][centerY] == UNCHECKEDVAL)
		{
			mineArray[centerX][centerY] = countNeighborBombs(mineArray, centerX, centerY);
			if (mineArray[centerX][centerY] == 0)
			{
				showMyCount(mineArray, centerX - 1, centerY - 1);
				showMyCount(mineArray, centerX - 1, centerY);
				showMyCount(mineArray, centerX - 1, centerY + 1);
				showMyCount(mineArray, centerX, centerY - 1);
				showMyCount(mineArray, centerX, centerY + 1);
				showMyCount(mineArray, centerX + 1, centerY - 1);
				showMyCount(mineArray, centerX + 1, centerY);
				showMyCount(mineArray, centerX + 1, centerY + 1);
			}
		}
	}
}

/* printBoard
Creates the entire board, indexs, border lines, fills empty spaces with border lines
gives text color on bomb count
Parameters: The board
*/
void printBoard(int mineArray[MAX][MAX])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);		// for the colors

	SetConsoleTextAttribute(hConsole, YELLOW);			// Colors in top row indexs
	cout << endl << "     ";
	for (int count = 0; count < MAX; count++)
	{
		cout << setw(4) << count;
	}
	cout << endl;


	SetConsoleTextAttribute(hConsole, YELLOW);			// Top
	for (int i = 0; i < 45; i++)
	{
		if (i == 44)
		{
			
			cout << char(191);
		}
		else if (i == 4)
		{
			cout << char(218);
		}
		else if (i > 4)
		{
			cout << char(196);
		}
		else
		{
			cout << ' ';
		}
	}
	cout << endl;
	cout << setw(5) << char(179) << setw(40) << char(179) << endl;


	for (int y = 0; y < MAX; y++)
	{
		SetConsoleTextAttribute(hConsole, YELLOW);
		cout << " " << y << "  " << char(179);
		SetConsoleTextAttribute(hConsole, BLUE);			
		for (int x = 0; x < MAX; x++)
		{
			if (mineArray[x][y] == UNCHECKEDVAL || mineArray[x][y] == BOMBVALUE)
			{
				cout << setw(4) << char(42);
			}
			else if (mineArray[x][y] < 0)
			{
				SetConsoleTextAttribute(hConsole, RED);
				cout << setw(4) << 'F';
				SetConsoleTextAttribute(hConsole, BLUE);
			}
			else if (mineArray[x][y] == 0)
			{
				cout << setw(4) << char(000);
			}
			else
			{
				cout << setw(4) << mineArray[x][y];
			}
		}
		cout << "   ";
		SetConsoleTextAttribute(hConsole, YELLOW);				// Right Border 
		cout << char(179) << endl;
		cout << setw(5) << char(179) << setw(40) << char(179) << endl;		// (Empty Row)
	}
	SetConsoleTextAttribute(hConsole, YELLOW);			// Bottom
	for (int i = 0; i < 45; i++)
	{
		if (i == 44)
		{

			cout << char(217);
		}
		else if (i == 4)
		{
			cout << char(192);
		}
		else if (i > 4)
		{
			cout << char(196);
		}
		else
		{
			cout << ' ';
		}
	}
	SetConsoleTextAttribute(hConsole, GREEN);
	cout << endl << "There are " << bombCount(mineArray) << " bombs left." << endl;
}

/* gameOverCheck
Checks if the game is over.  Seeing if everything except for the bombs has been fliped over
Parameters: The board
return: game over (y/n)
*/
bool gameOverCheck(int mineArray[MAX][MAX])
{
	bool keepG = false;		// for keep going

	for (int x = 0; x < MAX; x++)
	{
		for (int y = 0; y < MAX && keepG == false; y++)
		{					
			if (mineArray[x][y] == UNCHECKEDVAL || mineArray[x][y] == -UNCHECKEDVAL) /* checks for unflipped cells
																					     and incorrect flags  */
			{															
				keepG = true;	// If there are none, keep playing
			}
		}
	}
	return keepG;
}

/* bombCount
number of bombs left unflagged
parameter: the board
return: number of bombs
*/
int bombCount(int mineArray[MAX][MAX])
{
	int count = 10;	// Ten bombs on board

	for (int x = 0; x < MAX; x++)
	{
		for (int y = 0; y < MAX; y++)
		{
			if (mineArray[x][y] < 0)		// checks for flags
			{
				count--;
			}
		}
	}
	return count;
}

/* winSmile
In the event of a win. The program will display an image of a happy face.
parameter: void
*/
void winSmile()														//Should you win the game.  Get a Smiley!
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, PINK);
	cout << setw(6) << char(220) << setw(7) << char(220) << endl << endl;
	cout << setw(9) << char(179) << endl;
	cout << setw(5) << char(236) << setw(9) << char(236) << endl;
	cout << setw(6) << char(236) << setw(7) << char(236) << endl;
	cout << setw(8) << char(236) << setw(3) << char(236) << endl;
	SetConsoleTextAttribute(hConsole, GREEN);
}

/* loseFrown
In the event of an explosion! The program will display a sorry sap.
Parameters: Void
*/
void loseFrown()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, PINK);
	cout << setw(4) << char(196) << setw(10) << char(196) << endl;
	cout << setw(9) << char(179) << endl;
	cout << endl;
	cout << setw(9) << char(236) << endl;
	cout << setw(6) << char(236) << setw(6) << char(236) << endl;
	cout << setw(4) << char(236) << setw(10) << char(236) << endl;
	SetConsoleTextAttribute(hConsole, GREEN);
}
