/*
Isaac Haas
Mancala 9
12/4/2015
This will let users play Mancala!
It first asks the player whether they want to play vs computer, or vs someone else.
If they choose the computer, it will check for a few obvious moves; If there aren't any, it will move the biggest bin.
It asks the player what bin they want to start in.
It will make sure the bin is on the correct side and that there are beads in it.
Then it will increment the following bins untill the beads run out.
When the player lands in an empty bin, their turn ends.
This repeats untill one side of te board is empty.
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <Windows.h>
using namespace std;
const int MAX = 14;				// Size of beadArray

void makeSolidLine( int stars );		// Solid line of stars
void makeDottedLine( );					// Dotted line of stars
void makeMiddleLine(HANDLE hConsole);				//	Solid line of stars for middle of board
void showTopRowNumbers(HANDLE hConsole);			// The top bin numbers
void showBottomRowNumbers(HANDLE hConsole);			// The bottom bin numbers
void showBoard(int beadArry[MAX], HANDLE hConsole);				// Prints the outline of mancala board
void startingArray(int beadArray[MAX]);							// Fills beadArray with 4s and 0s
void printArray(int beadArray[MAX]);							// Prints beadArray to screen
void showTopBins(int beadArray[MAX],HANDLE hConsole);			// Prints first 6 numbers in beadArray to the board
void showBottomBins(int beadArray[MAX], HANDLE hConsole);		// Prints numbers 8-13 in beadArray to the board
int gameOverCheck(int beadArray[MAX]);							// Checks if game ended. Returns a value for continue, win or tie.
void getStaringBin(int beadArray[MAX], string player, int &binChoice);						// Promt for starting bin
void dropBeads(int beadArray[MAX], string player, int &binChoice, int &emptyCheck);		// Plays the game, pretty much.
void clearBoard(int beadArray[MAX], HANDLE hConsole);									// Clears the beads from middle bins
void printResults(int check);						// Prints the winner
int AIBinChoice(int beadArray[MAX]);				// Chooses a bin

int main()
{
	HANDLE hConsole;				// These are for the couts
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int beadArray[MAX];					// Array of 14 intagers for the beads
	int check = 0;							// For verifying the function gameOverCheck
	string player = "Player one";		// Which player is playing
	int binChoice;							// Bin to start in
	int emptyCheck = 0;					// To check if turn is over
	bool AI = false;						// vs computer: true, vs human: false
	bool firstTime = true;				// Aids in smart output for player

	//			promt for AI or two-player
	cout << "To play vs the computer, enter a 1. For two players, enter a 0: ";
	cin >> AI;

	startingArray(beadArray);

	do
	{

		showBoard(beadArray, hConsole);
		getStaringBin(beadArray, player, binChoice);
		dropBeads(beadArray, player, binChoice, emptyCheck);
		showBoard(beadArray, hConsole);
		check = gameOverCheck(beadArray);

		while (beadArray[binChoice] > 0 && check == -1 && emptyCheck == 0)	// This is the turn.
		{
			if (binChoice == 6 || binChoice == 13)
			{
				showBoard(beadArray, hConsole);
				getStaringBin(beadArray, player, binChoice);
				firstTime = true;
			}
			if (!firstTime)
			{
				cout << player << " is picking up bin " << binChoice << endl;
				system("pause");
			}
			dropBeads(beadArray, player, binChoice, emptyCheck);
			showBoard(beadArray, hConsole);
			check = gameOverCheck(beadArray);
			firstTime = false;
		}

		if (player == "Player one" && AI)
		{
			player = "The Computer";
		}
		else if (player == "Player one" && AI == false)
		{
			player = "Player two";
		}
		else
		{
			player = "Player one";
		}


	} while (check == -1);		// This is the entire game

	clearBoard(beadArray, hConsole);
	//printResults(check);
	if (!AI)
	{
		cout << player << " has won the game!!\n";
	}
	else
	{
		if (check == 1)
		{
			cout << "You have defeated the computer!!\n";
		}
		else
		{
			cout << "The computer won this round\n";
		}
	}

	cout << endl;
	system("pause");
	return 0;
}

/* makeSolidLine
creates a solid line of asteriks
parameter: number of stars in the line
*/
void makeSolidLine(int stars)
{
	for (int count = 0; count < stars; count++)
	{
		cout << char(219);
	}
}

/* makeDottedLine
creates a dotted line of asteriks
parameter: void
*/
void makeDottedLine()
{
	for (int starCount = 0; starCount < 8; starCount++)
	{
		cout << char(219);

		for (int spaceCount = 0; spaceCount < 6; spaceCount++)
		{
			cout << " ";
		}
	}
	cout << char(219) << endl;
}

/* makeMiddleLine
creates the middle line of the board with the numbers of the end bins
parameter: void
*/
void makeMiddleLine(HANDLE hConsole)
{
	cout << char(219);
	SetConsoleTextAttribute(hConsole, 26); 
	cout << setw(4) << "13";
	SetConsoleTextAttribute(hConsole, 24);
	cout << "  ";
	makeSolidLine(43);
	SetConsoleTextAttribute(hConsole, 27);
	cout << setw(4) << 6;
	SetConsoleTextAttribute(hConsole, 24); 
	cout << setw(3) << char(219);
	cout << endl;
}

/* showTopRowNumbers
prints the numbers for the top row of bins
parameter: void
*/
void showTopRowNumbers(HANDLE hConsole)
{
	cout << char(219) << setw(7) << char(219);
	for (int i = 0; i < 6; i++)
	{
		SetConsoleTextAttribute(hConsole, 27);
		cout << setw(4) << i;
		SetConsoleTextAttribute(hConsole, 24);
		cout << setw(3) << char(219);
	}
	cout << setw(7) << char(219) << endl;
}

/* showBottomRowNumbers
prints the numbers for the bottom row of bins
parameter: void
*/
void showBottomRowNumbers(HANDLE hConsole)
{
	cout << char(219) << setw(7) << char(219);
	for (int i = 12; i > 6; i--)
	{
		SetConsoleTextAttribute(hConsole, 26);
		cout << setw(4) << i;
		SetConsoleTextAttribute(hConsole, 24); 
		cout << setw(3) << char(219);
	}
	cout << setw(7) << char(219) << endl;
}

/* showBoard
prints the board to the screen with the numbers of the bins and beads
parameter: beadArray-for the functions "showTopBins" and "showBottomBins"
*/
void showBoard(int beadArray[MAX], HANDLE hConsole)
{
	system("cls");

	SetConsoleTextAttribute(hConsole, 24);
	makeSolidLine(57);
	cout << endl;
	makeDottedLine();
	showTopRowNumbers(hConsole);
	makeDottedLine();
	showTopBins(beadArray, hConsole);
	makeDottedLine();
	makeMiddleLine(hConsole);
	makeDottedLine();
	showBottomRowNumbers(hConsole);
	makeDottedLine();
	showBottomBins(beadArray, hConsole);
	makeDottedLine();
	makeSolidLine(57);
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl;
	cout << endl;
	//system("pause");
}

/* startingArray
Fills an array with 14 fours and 2 zeros
parameter: beadArray-to fill beadArray
*/
void startingArray(int beadArray[MAX])
{
	for (int i = 0; i < MAX; i++)
	{
		beadArray[i] = 4;
	}
	beadArray[6] = 0;
	beadArray[13] = 0;

}

/* printArray
prints an array to the console
parameter: beadArray-to print it to console
*/
void printArray(int beadArray[MAX])
{
	for (int i = 0; i < MAX; i++)
	{
		cout << beadArray[i] << endl;
	}
	cout << endl;
}

/* showTopBins
prints the first 6 numbers of an array to the macala board
parameter: beadArray-for the num of beads in first 5 indexes
*/
void showTopBins(int beadArray[MAX], HANDLE hConsole)
{
	cout << char(219) << setw(7) << char(219);
	for (int i = 0; i < 6; i++)
	{
		SetConsoleTextAttribute(hConsole, 30);
		cout << setw(4) << beadArray[i];
		SetConsoleTextAttribute(hConsole, 24);
		cout << setw(3) << char(219);
	}
	cout << setw(7) << char(219) << endl;
}

/* showBottomBins
prints 7th through the 12th indexes to the mancala board
parameter: beadArray-for num of beads in 7th through the 12th indexes
*/
void showBottomBins(int beadArray[MAX], HANDLE hConsole)
{
	cout << char(219);
	for (int i = 13; i > 5; i--)
	{
		SetConsoleTextAttribute(hConsole, 30);
		cout << setw(4) << beadArray[i];
		SetConsoleTextAttribute(hConsole, 24);
		cout << setw(3) << char(219);
	}
	cout << endl;
}

/* gameOverCheck
determines if all bins on one side are empty, and which player has more beads.
parameter: beadArray which allows us to count the beads
return: -1 if game continues, 1 if 1player won, 2 if 2player won, or 3 for tie
*/
int gameOverCheck(int beadArray[MAX])
{
	int player1Total = 0;		// player one's beads
	int player2Total = 0;		// player two's beads
	int returnValue = -1;		// value to be returned to main

	for (int i = 0; i < 6; i++)
	{
		player1Total += beadArray[i];
	}
	for (int i = 12; i > 6; i--)
	{
		player2Total += beadArray[i];
	}

	if (player1Total == 0 || player2Total == 0)
	{
		beadArray[6] += player2Total;
		beadArray[13] += player1Total;

		if (beadArray[6] == beadArray[13])
		{
			// tie game
			returnValue = 3;
		}
		else if (beadArray[6] > beadArray[13])
		{
			// player 1 wins
			returnValue = 1;
		}
		else
		{
			// player 2 wins
			returnValue = 2;
		}
	}

	return returnValue;
}

/* getStartingBin
Asks user what bin to start in. Verifies choice, and passes it back through reference
parameter: number of beads on board, which player is playing, and the number of the bin the player chose
*/
void getStaringBin(int beadArray[MAX], string player, int &binChoice)
{
	int lowBin;			// The low range
	int highBin;		// High range

	if (player == "Player one")
	{
		lowBin = 0;
		highBin = 5;
	}
	else
	{
		lowBin = 7;
		highBin = 12;
	}

	if (player != "The Computer")
	{
		cout << player << " please choose a bin between " << lowBin << " and " << highBin << ": ";
		cin >> binChoice;
	}
	else
	{
		binChoice = AIBinChoice(beadArray);
	}

	while (beadArray[binChoice] == 0 || binChoice < lowBin || binChoice > highBin)
	{
		cout << "The bin you chose is invalid. Please choose a new bin: ";
		cin >> binChoice;
	}
}

/* dropBeads
Takes the beads from a bin and increments the following bins until the beads run out
parameter: number of beads on board, who is playing, and the bin that the user wants to start in
*/
void dropBeads(int beadArray[MAX], string player, int &binChoice, int &emptyCheck)
{
	int hand;					// Number of beads to be dropped
	int oppEndBin;				// End bin of the opponent
	int playerEndBin;			// End bin of user

	emptyCheck = 0;				// To check if turn is over

	hand = beadArray[binChoice];
	beadArray[binChoice] = 0;

	if (player == "Player one")
	{
		oppEndBin = 13;
		playerEndBin = 6;
	}
	else
	{
		oppEndBin = 6;
		playerEndBin = 13;
	}

	while (hand > 0)
	{
		binChoice++;
		if (binChoice == oppEndBin)
		{
			binChoice++;
		}
		if (binChoice > 13)
		{
			binChoice = 0;
		}
		beadArray[binChoice]++;
		hand--;

	}
	if (beadArray[binChoice] == 1 && binChoice != 6 && binChoice != 13)		// This checks if the turn is over
	{
		emptyCheck = 1;
	}
}

/* clearBoard
When game ends, this prints out an empty board
parameter: The beads on the board
*/
void clearBoard(int beadArray[MAX], HANDLE hConsole)
{
	for (int i = 0; i < 6; i++)
	{
		beadArray[i] = 0;
	}
	for (int i = 12; i > 6; i--)
	{
		beadArray[i] = 0;
	}
	showBoard(beadArray, hConsole);
}

/* printResults
Prints the who won the game, or a tie
parameter: number of winner, or tie
*/
void printResults(int check)
{
	cout << endl;
	if (check == 1)
	{
		cout << setw(3) << "Player 1 won";
	}
	else if (check == 2)
	{
		cout << setw(3) << "Player 2 won";
	}
	else
	{
		cout << setw(3) << "You tied";
	}
	cout << endl;
}

/* AIBinChoice
This makes a decent choice of bin for a one-player game
parameter: beads to choose from
returns: computer's bin choice
*/
int AIBinChoice(int beadArray[MAX])
{
	bool cont = true;				// continue if choice has not been made
	int binChoice = 12;				// computer's choice of index
	int index = 12;					// index of computer's choice
	int biggest = beadArray[12];	// biggest pile of beads

	while (cont && index > 6)	// This checks each bin
	{
		//		if beads will land in end bin
		if (beadArray[index] + index == 13)
		{
			binChoice = index;
			cont = false;		// By setting to false, we know the computer found a move
		}
		//			This looks two moves ahead to determine if it will land in endbin. I know this is extreamly ugly, however, it works. 
		else if (beadArray[beadArray[index] + index] + 1 + (beadArray[index] + index) == 13 && beadArray[beadArray[index] + index] != 0)
		{
			binChoice = index;
			cont = false;		// Same as above: the computer found a move
		}
		else//	 If no move, check next index
		{
			index--;
		}
	}

	if (cont)		//if there was no choice, it finds the biggest bin
	{

		for (int i = 12; i > 6; i--)
		{
			if (beadArray[i] > biggest)
			{
				biggest = beadArray[i];
				binChoice = i;
			}
		}
	}

	cout << endl << "The computer chose: " << binChoice << endl;
	system("pause");
	return binChoice;		// The computers choice of index
}



// Problems: none