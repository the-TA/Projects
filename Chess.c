/******************************************************************************************************************************
* Authors:		Isaac, Israel, Ian
* Title:			Chess
* Due:			5/8/2017
* Language:		C
* Description:	Two player chess. The board is a 2-D array of characters. For each piece we use bit manipulation to store the type of piece,
					which team, if it has been moved, and if it was the last piece to move.
*******************************************************************************************************************************/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef int bool;			// making dem flags

#define PIECE_BITS 7		// What type of piece 0000 0XXX
#define MOVED_BIT 16		// activated if the piece has been moved 000X 0000
#define HIGHLIGHT_BIT 32 // activated to show last move 0X00 0000
#define PLAYER_1_BIT 8	// 1 for player 1, 0 for player 2 0000 X000

#define true 1				//settin dem truths
#define false 0			//settin dem falacies

#define BOARD_SIZE 8		//ze size of das board

#define PAWN 1				//0x01
#define ROOK 2				//0x02
#define NIGHT 3			//0x03
#define BISHOP 4			//0x04
#define QUEEN 5			//0x05
#define KING 6				//0x06

//Macro for finding if the players bit matches
#define IF_PLAYER_MATCH(piece, p1Bool) ((p1Bool && (piece & PLAYER_1_BIT)) ? true : (((!p1Bool) && !(piece & PLAYER_1_BIT)) ? true : false))

//----------------------------------------PROTOTYPES LIVE HERE-------------------------------------//

bool checkCheck(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1);
bool checkSquare(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1, int y, int x);
bool fakeMove(char board[BOARD_SIZE][BOARD_SIZE], int* yInitial, int* xInitial, int* yFinal, int* xFinal, bool isPlayer1);
int findAttackers(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1, int* aY, int* aX);
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool myFakeMove(char board[BOARD_SIZE][BOARD_SIZE], int yFrom, int xFrom, int yTo, int xTo, bool isPlayer1);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE], bool player1);
void realMove(char board[BOARD_SIZE][BOARD_SIZE], int* yInitial, int* xInitial, int* yFinal, int* xFinal, bool isPlayer1);
bool userInput(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1, int* yInitial, int* xInitial, int* yFinal, int* xFinal);
bool validMove(char board[BOARD_SIZE][BOARD_SIZE], int* yInitial, int* xInitial, int* yFinal, int* xFinal);


//----------------------------------------THIS IS WHERE THE MAGIC HAPPENS-------------------------------------//
int main()
{
	// these are the flags we use 
	bool isPlayer1 = true;							//is the player1 bit flipped
	bool isCheckMate = false;						//is the player in checkmate
	bool kingCanMove = false;						//can the king move
	bool valid = false;								//is this a valid move
	bool kingFound = false;							//is there a king

	//vairable list starts here			
	int kingRow, kingCol;								//kings coordinates on the board
	int* xInitial = (int*)malloc(sizeof(int));	//the x value for start location
	int* yInitial = (int*)malloc(sizeof(int));	//the y value for start location
	int* xFinal = (int*)malloc(sizeof(int));		//the x value for end location
	int* yFinal = (int*)malloc(sizeof(int));		//the y value for end location
	int aY = 0, aX = 0;									// the locations of the attacking piece
	int yDif, xDif;										// to loop through attack paths
	int numAttackers;										//the number of attackers
	char board[BOARD_SIZE][BOARD_SIZE];				//the master board 


	initializeBoard(board);								//setting up the board
	printBoard(board, isPlayer1);						//printing it for the first time

	//+------------------------------- Game Loop   ----------------------------------------+
	while (!isCheckMate)									//while not in checkmate
	{
		valid = false;										//setting the flag to false
		while (!valid)										//start of moving a piece, while flag is false, go through userInput, fakeMove, and validMove
		{
			valid = userInput(board, isPlayer1, yInitial, xInitial, yFinal, xFinal);		
			if (valid = fakeMove(board, yInitial, xInitial, yFinal, xFinal, isPlayer1))	 
			{
				if (valid = validMove(board, yInitial, xInitial, yFinal, xFinal))
				{
					realMove(board, yInitial, xInitial, yFinal, xFinal, isPlayer1);	//if all cases of a valid move are true, move piece
				}
			}
		}														//end of moving a piece

		isPlayer1 = !isPlayer1;	// switch players 

			//---this block is here to for deleting the highlight bit--
		printBoard(board, isPlayer1);
		board[*yFinal][*xFinal] = board[*yFinal][*xFinal] - HIGHLIGHT_BIT;
		board[*yInitial][*xInitial] = 0;
			//---end of highlight block--

		//++---------------start checking for checkmate-----------------++//
		// loop through the board to find king, escape if king is found
		kingFound = false;
		for (int r = 0; r < BOARD_SIZE && !kingFound; r++)
		{
			for (int c = 0; c < BOARD_SIZE && !kingFound; c++)
			{
				if ((board[r][c] & PIECE_BITS) == KING && IF_PLAYER_MATCH(board[r][c], isPlayer1)) //if the board location contains the king for the right player
				{
					kingRow = r;
					kingCol = c;
					kingFound = true;
				}
			}
		}

		//if king not found, the game be broken
		if (!kingFound) 
		{
			printf("\nno king found: player %d\n", isPlayer1 ? 1 : 2);
			exit(-1);
		}

		// check can king move
		kingCanMove = false;
		for (int i = -1; i < 2 && !kingCanMove; i++)
		{
			for (int j = -1; j < 2 && !kingCanMove; j++)
			{
				if (((kingRow + i > 0) && (kingRow + i < 8) && (kingCol + j > 0 && kingCol + j < 8)) && // bounds
					((board[i][j] == 0) || !(IF_PLAYER_MATCH(board[i][j], isPlayer1)))) // not friend
				{
					if (myFakeMove(board, kingRow, kingCol, kingRow + i, kingCol + j, isPlayer1))//Isaac's function to figure out if move is valid for board
					{																								// we already know that the move is valid for a king 
						kingCanMove = true;
					}
				}
			}
		}

		// if king can't move, might be in checkmate
		if (!kingCanMove)
		{
			numAttackers = findAttackers(board, isPlayer1, &aY, &aX);
			if (numAttackers < 1) // not in check
			{
				isCheckMate = false;
			}
			else if (numAttackers > 1) // multiple checks/attackers
			{
				isCheckMate = true;
			}
			else // one attacker: can it be blocked?
			{
				bool flag = false;

				// the one attacker is a knight
				if ((board[aY][aX] & PIECE_BITS) == NIGHT)
				{
					isCheckMate = !checkSquare(board, isPlayer1, aY, aX);
				}
				else
				{
					yDif = aY - kingRow;
					xDif = aX - kingCol;

					// these are each of the 8 paths that the attack could be from
					if (yDif > 0)
					{
						if (xDif > 0)
						{
							for (int y = kingRow + 1, x = kingCol + 1; y <= aY && x <= aX && !flag; y++, x++)
							{
								flag = checkSquare(board, isPlayer1, y, x);	// attack from top right corner
							}
						}
						else if (xDif < 0)
						{
							for (int y = kingRow + 1, x = kingCol - 1; y <= aY && x >= aX && !flag; y++, x--)
							{
								flag = checkSquare(board, isPlayer1, y, x);	// attack from right
							}
						}
						else	// xDif = 0, so the attack is chess
						{
							for (int y = kingRow + 1; y <= aY && !flag; y++)
							{
								flag = checkSquare(board, isPlayer1, y, aX);	// attack from above
							}
						}
					}// end of if (yDif > 0)
					else if (yDif < 0)
					{
						if (xDif > 0)
						{
							for (int y = kingRow - 1, x = kingCol + 1; y >= aY && x <= aX && !flag; y--, x++)
							{
								flag = checkSquare(board, isPlayer1, y, x);	// attack from bottom right
							}
						}
						else if (xDif < 0)
						{
							for (int y = kingRow - 1, x = kingCol - 1; y >= aY && x >= aX && !flag; y--, x--)
							{
								flag = checkSquare(board, isPlayer1, y, x);	// attack from bottom left
							}
						}
						else	// xDif = 0, so the attack is vertical
						{
							for (int y = kingRow - 1; y >= aY && !flag; y--)
							{
								flag = checkSquare(board, isPlayer1, y, aX);	// attack from bottom
							}
						}
					} // end else if (yDif < 0)
					else // attack is horizontal
					{
						if (xDif > 0)
						{
							for (int x = kingCol + 1; x <= aX && !flag; x++) // attack from right
							{
								flag = checkSquare(board, isPlayer1, aY, x);
							}
						}
						else if (xDif < 0)
						{
							for (int x = kingCol - 1; x >= aX && !flag; x--) // attack from left
							{
								flag = checkSquare(board, isPlayer1, aY, x);
							}
						}
					}

					// flag equals canBlock, so checkmate is the opposite
					isCheckMate = !flag;

				}// end of else from if(numAttackers...){}
			}
		}
		if (isCheckMate)
		{
			printf("\n\nCHECKMATE!!!!!\n");
		} //++-----------------end of check mate loop-----------------++

	} //+------------------ end of game loop -------------------------+

	//----this block frees the memory malloc for pointers----
	free(xInitial);
	free(yInitial);
	free(xFinal);
	free(yFinal);

	printf("\n\n");		//for vizual reasons
	system("pause");
	return 0;
}

//----------------------------------------LOOKS FOR WHO CAN/IS PUTTING THE KING IN CHECK-------------------------------------//
bool checkCheck(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1)
{
	int kingRow = -1, kingCol = -1;	//one down and one left for the kings coordinates
	int r, c;								//these are some vairables 
	char piece;								//temp storage of a piece
	bool inCheck = false;				//flag for in check or not
	bool kingFound = false;				//flag for king is found or not

	//checking for the king through the whole board
	for (int r = 0; r < BOARD_SIZE && !kingFound; r++)
	{
		for (int c = 0; c < BOARD_SIZE && !kingFound; c++)
		{
			if ((board[r][c] & PIECE_BITS) == KING && IF_PLAYER_MATCH(board[r][c], isPlayer1))
			{
				kingRow = r;
				kingCol = c;
				kingFound = true;
			}
		}
	}
	if (kingRow == -1 || kingCol == -1)
	{
		printf("Couldn't find king!!!");
		system("pause");
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// ***********************************************************************   LOOK FOR KNIGHT   ***********************************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	if (kingRow + 2 < BOARD_SIZE && kingCol + 1 < BOARD_SIZE && (board[kingRow + 2][kingCol + 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 2][kingCol + 1], isPlayer1))
		inCheck = true;
	else if (kingRow + 2 < BOARD_SIZE && kingCol - 1 < BOARD_SIZE && (board[kingRow + 2][kingCol - 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 2][kingCol - 1], isPlayer1))
		inCheck = true;
	else if (kingRow + 1 < BOARD_SIZE && kingCol + 2 < BOARD_SIZE && (board[kingRow + 1][kingCol + 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 1][kingCol + 2], isPlayer1))
		inCheck = true;
	else if (kingRow + 1 < BOARD_SIZE && kingCol - 2 < BOARD_SIZE && (board[kingRow + 1][kingCol - 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 1][kingCol - 2], isPlayer1))
		inCheck = true;
	else if (kingRow - 1 < BOARD_SIZE && kingCol + 2 < BOARD_SIZE && (board[kingRow - 1][kingCol + 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 1][kingCol + 2], isPlayer1))
		inCheck = true;
	else if (kingRow - 1 < BOARD_SIZE && kingCol - 2 < BOARD_SIZE && (board[kingRow - 1][kingCol - 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 1][kingCol - 2], isPlayer1))
		inCheck = true;
	else if (kingRow - 2 < BOARD_SIZE && kingCol + 1 < BOARD_SIZE && (board[kingRow - 2][kingCol + 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 2][kingCol + 1], isPlayer1))
		inCheck = true;
	else if (kingRow - 2 < BOARD_SIZE && kingCol - 1 < BOARD_SIZE && (board[kingRow - 2][kingCol - 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 2][kingCol - 1], isPlayer1))
		inCheck = true;

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   LOOKING FOR ROOK OR QUEEN   ******************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	for (r = kingRow + 1; r < BOARD_SIZE && board[r][kingCol] == 0; r++) { ; } // looking forward
	if (r < BOARD_SIZE) // then it found a piece
	{
		piece = board[r][kingCol];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (r = kingRow - 1; r >= 0 && board[r][kingCol] == 0; r--) { ; } // looking backward
	if (r >= 0) // then it found a piece
	{
		piece = board[r][kingCol];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (c = kingCol - 1; c >= 0 && board[kingRow][c] == 0; c--) { ; } // looking left
	if (c >= 0) // then it found a piece
	{
		piece = board[kingRow][c];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (c = kingCol + 1; c < BOARD_SIZE && board[kingRow][c] == 0; c++) { ; } // looking right
	if (c >= 0) // then it found a piece
	{
		piece = board[kingRow][c];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   END OF LOOKING FOR ROOK OR QUEEN   ***********************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   LOOKING FOR BISHOP OR QUEEN   ****************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	for (r = kingRow + 1, c = kingCol - 1; r < BOARD_SIZE && c >= 0 && board[r][c] == 0; r++, c--) { ; } // looking forward-left digonal
	if (c >= 0 && r < BOARD_SIZE) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow + 1) && (c == kingCol - 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN)
		{
			inCheck = true;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}


	for (r = kingRow + 1, c = kingCol + 1; r < BOARD_SIZE && c < BOARD_SIZE && board[r][c] == 0; r++, c++) { ; } // looking forward-right digonal
	if (c < BOARD_SIZE && r < BOARD_SIZE) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow + 1) && (c == kingCol + 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN)
		{
			inCheck = true;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}


	for (r = kingRow - 1, c = kingCol + 1; r >= 0 && c < BOARD_SIZE && board[r][c] == 0; r--, c++) { ; } // looking backward-right digonal
	if (c < BOARD_SIZE && r >= 0) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow - 1) && (c == kingCol + 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN + PLAYER_1_BIT)
		{
			inCheck = true;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}


	for (r = kingRow - 1, c = kingCol - 1; r >= 0 && c >= 0 && board[r][c] == 0; r--, c--) { ; } // looking backward-left digonal
	if (c >= 0 && r >= 0) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow - 1) && (c == kingCol - 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN + PLAYER_1_BIT)
		{
			inCheck = true;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   END OF LOOKING FOR BISHOP OR QUEEN   ***********************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________


	return inCheck;
}

//----------------------------------------LOOKS AT THE GIVEN SQUARE ON THE BOARD TO SEE WHO CAN PATH THERE-------------------------------------//
bool checkSquare(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1, int y, int x)
{
	int r, c;
	char piece;
	bool canBlock = false;

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// ***********************************************************************   LOOK FOR KNIGHT   ***********************************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	if (y + 2 < BOARD_SIZE && x + 1 < BOARD_SIZE && (board[y + 2][x + 1] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y + 2][x + 1], isPlayer1))
	{
		if (myFakeMove(board, y + 2, x + 1, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y + 2 < BOARD_SIZE && x - 1 < BOARD_SIZE && (board[y + 2][x - 1] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y + 2][x - 1], isPlayer1))
	{
		if (myFakeMove(board, y + 2, x - 1, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y + 1 < BOARD_SIZE && x + 2 < BOARD_SIZE && (board[y + 1][x + 2] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y + 1][x + 2], isPlayer1))
	{
		if (myFakeMove(board, y + 1, x + 2, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y + 1 < BOARD_SIZE && x - 2 < BOARD_SIZE && (board[y + 1][x - 2] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y + 1][x - 2], isPlayer1))
	{
		if (myFakeMove(board, y + 1, x - 2, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y - 1 < BOARD_SIZE && x + 2 < BOARD_SIZE && (board[y - 1][x + 2] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y - 1][x + 2], isPlayer1))
	{
		if (myFakeMove(board, y - 1, x + 2, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y - 1 < BOARD_SIZE && x - 2 < BOARD_SIZE && (board[y - 1][x - 2] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y - 1][x - 2], isPlayer1))
	{
		if (myFakeMove(board, y - 1, x - 2, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y - 2 < BOARD_SIZE && x + 1 < BOARD_SIZE && (board[y - 2][x + 1] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y - 2][x + 1], isPlayer1))
	{
		if (myFakeMove(board, y - 2, x + 1, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}
	else if (y - 2 < BOARD_SIZE && x - 1 < BOARD_SIZE && (board[y - 2][x - 1] & PIECE_BITS) == NIGHT && IF_PLAYER_MATCH(board[y - 2][x - 1], isPlayer1))
	{
		if (myFakeMove(board, y - 2, x - 1, y, x, isPlayer1))
		{
			canBlock = true;
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   LOOKING FOR ROOK OR QUEEN   ******************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	for (r = y + 1; r < BOARD_SIZE && board[r][x] == 0; r++) { ; } // looking forward
	if (r < BOARD_SIZE) // then it found a piece
	{
		piece = board[r][x];
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, r, x, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
		else
		{
			if (board[y][x] == 0)
			{
				if (!isPlayer1 && (piece & 15) == PAWN && (r - y == 1 || (r - y == 2 && !(piece & MOVED_BIT))))
				{
					if (myFakeMove(board, r, x, y, x, isPlayer1))
					{
						canBlock = true;
					}
				}
			}
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (r = y - 1; r >= 0 && board[r][x] == 0; r--) { ; } // looking backward
	if (r >= 0) // then it found a piece
	{
		piece = board[r][x];
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, r, x, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
		else
		{
			if (board[y][x] == 0)
			{
				if (isPlayer1 && (piece & 15) == (PAWN + 8) && (y - r == 1 || (y - r == 2 && !(piece & MOVED_BIT))))
				{
					if (myFakeMove(board, r, x, y, x, isPlayer1))
					{
						canBlock = true;
					}
				}
			}
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (c = x - 1; c >= 0 && board[y][c] == 0; c--) { ; } // looking left
	if (c >= 0) // then it found a piece
	{
		piece = board[y][c];
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, y, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (c = x + 1; c < BOARD_SIZE && board[y][c] == 0; c++) { ; } // looking right
	if (c >= 0) // then it found a piece
	{
		piece = board[y][c];
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, y, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   END OF LOOKING FOR ROOK OR QUEEN   ***********************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   LOOKING FOR BISHOP OR QUEEN   ****************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	for (r = y + 1, c = x - 1; r < BOARD_SIZE && c >= 0 && board[r][c] == 0; r++, c--) { ; } // looking forward-left digonal
	if (c >= 0 && r < BOARD_SIZE) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (board[y][x] != 0)
		{
			if (r == y + 1 && c == x - 1 && (IF_PLAYER_MATCH(piece, isPlayer1)) && (piece & 15) == PAWN)
			{
				if (myFakeMove(board, r, c, y, x, isPlayer1))
				{
					canBlock = true;
				}
			}
		}
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, r, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
	}


	for (r = y + 1, c = x + 1; r < BOARD_SIZE && c < BOARD_SIZE && board[r][c] == 0; r++, c++) { ; } // looking forward-right digonal
	if (c < BOARD_SIZE && r < BOARD_SIZE) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (board[y][x] != 0)
		{
			if (r == y + 1 && c == x + 1 && (IF_PLAYER_MATCH(piece, isPlayer1)) && (piece & 15) == PAWN)
			{
				if (myFakeMove(board, r, c, y, x, isPlayer1))
				{
					canBlock = true;
				}
			}
		}
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, r, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
	}


	for (r = y - 1, c = x + 1; r >= 0 && c < BOARD_SIZE && board[r][c] == 0; r--, c++) { ; } // looking backward-right digonal
	if (c < BOARD_SIZE && r >= 0) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == y - 1 && c == x + 1 && (IF_PLAYER_MATCH(piece, isPlayer1)) && (piece & 15) == PAWN + PLAYER_1_BIT)
		{
			if (myFakeMove(board, r, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, r, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
	}


	for (r = y - 1, c = x - 1; r >= 0 && c >= 0 && board[r][c] == 0; r--, c--) { ; } // looking backward-left digonal
	if (c >= 0 && r >= 0) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == y - 1 && c == x - 1 && (IF_PLAYER_MATCH(piece, isPlayer1)) && (piece & 15) == PAWN + PLAYER_1_BIT)
		{
			if (myFakeMove(board, r, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
		if ((IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			if (myFakeMove(board, r, c, y, x, isPlayer1))
			{
				canBlock = true;
			}
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   END OF LOOKING FOR BISHOP OR QUEEN   ***********************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	return canBlock;
}

//----------------------------------------A FAKE MOVE TO TEST IF IT IS VALID FOR THAT PIECE-------------------------------------//
bool fakeMove(char board[BOARD_SIZE][BOARD_SIZE], int* yInitial, int* xInitial, int* yFinal, int* xFinal, bool isPlayer1)
{
	char copyBoard[BOARD_SIZE][BOARD_SIZE];									//makes a copy array
	for (int row = BOARD_SIZE - 1; row >= 0; row--)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			copyBoard[row][column] = board[row][column];						//copies the board over to the copy array
		}
	}
	copyBoard[*yFinal][*xFinal] = copyBoard[*yInitial][*xInitial];		//moves the location on the copy array
	copyBoard[*yInitial][*xInitial] = 0;
	return !checkCheck(copyBoard, isPlayer1);									//preforms a checkCheck on the board
}

//----------------------------------------FINDS WHO IS ATTACKING THE KING-------------------------------------//
int findAttackers(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1, int* aY, int* aX)
{

	int attackerCount = 0;				//number of attackers function finds
	int kingRow, kingCol;				//location for king
	int r, c;								//temp row and column
	char piece;								//temp piece holder
	bool inCheck = false;				//flag for incheck
	bool kingFound = false;				//flag for if the king is found


	//finds the king
	for (int r = 0; r < BOARD_SIZE && !kingFound; r++)
	{
		for (int c = 0; c < BOARD_SIZE && !kingFound; c++)
		{
			if ((board[r][c] & PIECE_BITS) == KING && IF_PLAYER_MATCH(board[r][c], isPlayer1))
			{
				kingRow = r;
				kingCol = c;
				kingFound = true;
			}
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// ***********************************************************************   LOOK FOR KNIGHT   ***********************************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	if (kingRow + 2 < BOARD_SIZE && kingCol + 1 < BOARD_SIZE && (board[kingRow + 2][kingCol + 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 2][kingCol + 1], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow + 2;
		*aX = kingCol + 1;
		attackerCount++;
	}
	if (kingRow + 2 < BOARD_SIZE && kingCol - 1 < BOARD_SIZE && (board[kingRow + 2][kingCol - 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 2][kingCol - 1], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow + 2;
		*aX = kingCol - 1;
		attackerCount++;
	}
	if (kingRow + 1 < BOARD_SIZE && kingCol + 2 < BOARD_SIZE && (board[kingRow + 1][kingCol + 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 1][kingCol + 2], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow + 1;
		*aX = kingCol + 2;
		attackerCount++;
	}
	if (kingRow + 1 < BOARD_SIZE && kingCol - 2 < BOARD_SIZE && (board[kingRow + 1][kingCol - 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow + 1][kingCol - 2], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow + 1;
		*aX = kingCol - 2;
		attackerCount++;
	}
	if (kingRow - 1 < BOARD_SIZE && kingCol + 2 < BOARD_SIZE && (board[kingRow - 1][kingCol + 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 1][kingCol + 2], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow - 1;
		*aX = kingCol + 2;
		attackerCount++;
	}
	if (kingRow - 1 < BOARD_SIZE && kingCol - 2 < BOARD_SIZE && (board[kingRow - 1][kingCol - 2] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 1][kingCol - 2], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow - 1;
		*aX = kingCol - 2;
		attackerCount++;
	}
	if (kingRow - 2 < BOARD_SIZE && kingCol + 1 < BOARD_SIZE && (board[kingRow - 2][kingCol + 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 2][kingCol + 1], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow - 2;
		*aX = kingCol + 1;
		attackerCount++;
	}
	if (kingRow - 2 < BOARD_SIZE && kingCol - 1 < BOARD_SIZE && (board[kingRow - 2][kingCol - 1] & PIECE_BITS) == NIGHT && !IF_PLAYER_MATCH(board[kingRow - 2][kingCol - 1], isPlayer1))
	{
		inCheck = true;
		*aY = kingRow - 2;
		*aX = kingCol - 1;
		attackerCount++;
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   LOOKING FOR ROOK OR QUEEN   ******************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	for (r = kingRow + 1; r < BOARD_SIZE && board[r][kingCol] == 0; r++) { ; } // looking forward
	if (r < BOARD_SIZE) // then it found a piece
	{
		piece = board[r][kingCol];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = r;
			*aX = kingCol;
			attackerCount++;
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (r = kingRow - 1; r >= 0 && board[r][kingCol] == 0; r--) { ; } // looking backward
	if (r >= 0) // then it found a piece
	{
		piece = board[r][kingCol];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = r;
			*aX = kingCol;
			attackerCount++;
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (c = kingCol - 1; c >= 0 && board[kingRow][c] == 0; c--) { ; } // looking left
	if (c >= 0) // then it found a piece
	{
		piece = board[kingRow][c];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
	}

	//_____________________________________________________________________________________________________________________________________________________________________________________________
	for (c = kingCol + 1; c < BOARD_SIZE && board[kingRow][c] == 0; c++) { ; } // looking right
	if (c >= 0) // then it found a piece
	{
		piece = board[kingRow][c];
		if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == ROOK || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   END OF LOOKING FOR ROOK OR QUEEN   ***********************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   LOOKING FOR BISHOP OR QUEEN   ****************************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	for (r = kingRow + 1, c = kingCol - 1; r < BOARD_SIZE && c >= 0 && board[r][c] == 0; r++, c--) { ; } // looking forward-left digonal
	if (c >= 0 && r < BOARD_SIZE) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow + 1) && (c == kingCol - 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN)
		{
			inCheck = true;
			*aY = r;
			*aX = c;
			attackerCount++;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
	}


	for (r = kingRow + 1, c = kingCol + 1; r < BOARD_SIZE && c < BOARD_SIZE && board[r][c] == 0; r++, c++) { ; } // looking forward-right digonal
	if (c < BOARD_SIZE && r < BOARD_SIZE) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow + 1) && (c == kingCol + 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN)
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
	}


	for (r = kingRow - 1, c = kingCol + 1; r >= 0 && c < BOARD_SIZE && board[r][c] == 0; r--, c++) { ; } // looking backward-right digonal
	if (c < BOARD_SIZE && r >= 0) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow - 1) && (c == kingCol + 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN + PLAYER_1_BIT)
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
	}


	for (r = kingRow - 1, c = kingCol - 1; r >= 0 && c >= 0 && board[r][c] == 0; r--, c--) { ; } // looking backward-left digonal
	if (c >= 0 && r >= 0) // then it found a piece, friendly or otherwise
	{
		piece = board[r][c];
		if (r == (kingRow - 1) && (c == kingCol - 1) && (!IF_PLAYER_MATCH(piece, isPlayer1)) && piece == PAWN + PLAYER_1_BIT)
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
		else if ((!IF_PLAYER_MATCH(piece, isPlayer1)) && ((piece & PIECE_BITS) == BISHOP || (piece & PIECE_BITS) == QUEEN))
		{
			inCheck = true;
			*aY = kingRow;
			*aX = c;
			attackerCount++;
		}
	}
	//_____________________________________________________________________________________________________________________________________________________________________________________________
	// *******************************************************************************   END OF LOOKING FOR BISHOP OR QUEEN   ***********************************************************************
	//_____________________________________________________________________________________________________________________________________________________________________________________________

	//printBoard( board, true );
	//if (inCheck) printf( "num attackers = %d\nlocation = (%d, %c)", attackerCount, *aY + 1, *aX + 65 );
	//system( "pause" );

	return attackerCount;
}

//----------------------------------------INITIAL BOARD SETUP-------------------------------------//
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
	for (int row = BOARD_SIZE - 1; row >= 0; row--)
	{
		for (int column = 0; column < BOARD_SIZE; column++)
		{
			switch (row)	//switching on the row
			{
			case 7:	// player 2 King row
				if (column == 0 || column == 7) //rook
				{
					board[row][column] = ROOK;
				}
				else if (column == 1 || column == 6) // night
				{
					board[row][column] = NIGHT;
				}
				else if (column == 2 || column == 5) // BISHOP
				{
					board[row][column] = BISHOP;
				}
				else if (column == 3) // queen
				{
					board[row][column] = QUEEN;
				}
				else // king
				{
					board[row][column] = KING;
				}
				break;

			case 6:	// player 2 pawn row
				board[row][column] = PAWN;
				break;
			case 1:	// player 1 pawn row
				board[row][column] = PAWN + PLAYER_1_BIT;
				break;
			case 0:	// player 1 King row
				if (column == 0 || column == 7) //rook
				{
					board[row][column] = ROOK + PLAYER_1_BIT;
				}
				else if (column == 1 || column == 6) // night
				{
					board[row][column] = NIGHT + PLAYER_1_BIT;
				}
				else if (column == 2 || column == 5) // BISHOP
				{
					board[row][column] = BISHOP + PLAYER_1_BIT;
				}
				else if (column == 3) // queen
				{
					board[row][column] = QUEEN + PLAYER_1_BIT;
				}
				else // king
				{
					board[row][column] = KING + PLAYER_1_BIT;
				}

				break;
			default:
				board[row][column] = 0;	//anything not covered at this point is an empty space
				break;

			}
		}
	}
}

//----------------------------------------A DIFFERENT FAKE MOVE THAT CHECKS FOR PROPER INPUT WHILE NOT USING POINTERS-------------------------------------//
bool myFakeMove(char board[BOARD_SIZE][BOARD_SIZE], int yFrom, int xFrom, int yTo, int xTo, bool isPlayer1)
{
	/*********** myFakeMove   from Isaac  ************************|
	This ensures that the given move is valid for the board.
	*	It checks that the locations are not the same,
	*	That the final location is not a friendly piece,
	*	and that it won't put you in check
	**************************************************************/
	if (yFrom == yTo && xFrom == xTo)
	{
		return false;
	}
	else if (board[yTo][xTo] != 0 && (board[yFrom][xFrom] & PLAYER_1_BIT) == (board[yTo][xTo] & PLAYER_1_BIT))
	{
		return false;
	}
	else
	{
		char copyBoard[BOARD_SIZE][BOARD_SIZE];
		for (int row = BOARD_SIZE - 1; row >= 0; row--)
		{
			for (int column = 0; column < BOARD_SIZE; column++)
			{
				copyBoard[row][column] = board[row][column];
			}
		}
		copyBoard[yTo][xTo] = copyBoard[yFrom][xFrom];
		copyBoard[yFrom][xFrom] = 0;
		//printBoard( copyBoard, true );
		return !checkCheck(copyBoard, isPlayer1);
	}
}

//----------------------------------------A PRETTY VERSION OF PRINTING OUT THE ARRAY-------------------------------------//
void printBoard(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer)
{
	HANDLE hConsole;										//setting up the handle for the windows console input
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	//setting hConsole to the standard windows console input
	SetConsoleTextAttribute(hConsole, 8);			//sets the text color and background color of the text 0x08 is black background, silver text
	char translator[BOARD_SIZE][BOARD_SIZE];		//an array to translate the raw bits to char outputs
	char peice = ' ';

		//---this whole block copies the pieces over--
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			peice = ((board[i][j]) & 0x07);		//pulls everything down to just the pieces on the board
			switch (peice)
			{
			case PAWN:
				translator[i][j] = 'P';
				break;
			case ROOK:
				translator[i][j] = 'R';
				break;
			case NIGHT:
				translator[i][j] = 'N';
				break;
			case BISHOP:
				translator[i][j] = 'B';
				break;
			case QUEEN:
				translator[i][j] = 'Q';
				break;
			case KING:
				translator[i][j] = 'K';
				break;
			case 0:
				translator[i][j] = ' ';
				break;

			}
		}
	}
	//-------------------------------------THIS SETS UP THE TOP LINE OF THE BOARD----------------------------------
	SetConsoleTextAttribute(hConsole, 0x08);
	printf("\n   ");
	SetConsoleTextAttribute(hConsole, 0x80);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 218, 196, 196, 196, 194, 196, 196, 196, 194, 196, 196, 196, 194, 196, 196, 196, 194, 196, 196, 196, 194, 196, 196, 196, 194, 196, 196, 196, 194, 196, 196, 196, 191);
	//---------------------------------------------------------------------THIS BLOCK IS FOR PLAYER1 GRAPHICS FOR THE BOARD AND PERPECTIVE--------------------------------------------------------------------
	if (isPlayer)	
	{
		for (int j = 7; j >= 0; j--)
		{
			SetConsoleTextAttribute(hConsole, 0x08);
			printf(" %d ", j + 1);	//this is the numbers down the side of the board
			SetConsoleTextAttribute(hConsole, 0x80);
			printf("%c%c", 179, 0);//left edge of board
			//--------------------------One row of the board across besides the edge values
			for (int c = 0; c < BOARD_SIZE; c++)
			{
				if (board[j][c] & PLAYER_1_BIT)	//if this part of the board has the right players piece
				{
					SetConsoleTextAttribute(hConsole, 0x8f);	//sets the console to the right color
					if (board[j][c] & HIGHLIGHT_BIT)
					{
						SetConsoleTextAttribute(hConsole, 0x9f);	//sets the highlight color
					}
				}
				else  //not player1 bit
				{
					SetConsoleTextAttribute(hConsole, 0x84);	//other players color
					if (board[j][c] & HIGHLIGHT_BIT)
					{
						SetConsoleTextAttribute(hConsole, 0x94); //other players highlight color
					}
				}
				printf("%c", translator[j][c]);	//after the color is changed to the right sequence, print out the character for that space
				SetConsoleTextAttribute(hConsole, 0x80);	//sets the it back to the board color
				if (c < 7)	//to limit the number of lines it prints
				{
					printf("%c%c%c", 0, 179, 0);	//these are the vertical lines down the board
				}
			}
			printf("%c%c\n", 0, 179); //this sets up the last line and the new line character
			//-------------THIS MAKES THE HORIZONTAL LINES------------------------
			if (j > 0)
			{
				SetConsoleTextAttribute(hConsole, 0x08);
				printf("   ");
				SetConsoleTextAttribute(hConsole, 0x80);
				printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 195, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 180);
			}
		}
		//---------------THIS BLOCK SHOWS THE LAST HORIZONTAL LINE------------------
		SetConsoleTextAttribute(hConsole, 0x08);
		printf("   ");
		SetConsoleTextAttribute(hConsole, 0x80);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 192, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 217);
		SetConsoleTextAttribute(hConsole, 0x08);
		printf("   ");
		//---------------THIS IS THE LABELING FOR THE ALPHABETICAL AXIS------------------------
		for (int num = 65; num < 73; num++)
		{
			printf("  %c ", num);
		}
	}
	//------------------------------------------------------------------------------------SAME AS FOR PLAYER1 BUT NOW FOR NOT PLAYER1------------------------------------------------------------------------------------
	else  
	{
		for (int j = 0; j < BOARD_SIZE; j++)	//this is now reflected on the Y-Axis
		{
			SetConsoleTextAttribute(hConsole, 0x08);
			printf(" %d ", 8 - j);
			SetConsoleTextAttribute(hConsole, 0x80);
			printf("%c%c", 179, 0);
			for (int c = BOARD_SIZE - 1; c >= 0; c--)		//this is now reflected on the X-Axis
			{
				if (board[j][c] & PLAYER_1_BIT)
				{
					SetConsoleTextAttribute(hConsole, 0x8f);
					if (board[j][c] & HIGHLIGHT_BIT)
					{
						SetConsoleTextAttribute(hConsole, 0x9f);
					}
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 0x84);
					if (board[j][c] & HIGHLIGHT_BIT)
					{
						SetConsoleTextAttribute(hConsole, 0x94);
					}
				}
				printf("%c", translator[j][c]);
				SetConsoleTextAttribute(hConsole, 0x80);
				if (c > 0)
				{
					printf("%c%c%c", 0, 179, 0);
				}
			}
			printf("%c%c\n", 0, 179);
			if (j < 7)
			{
				SetConsoleTextAttribute(hConsole, 0x08);
				printf("   ");
				SetConsoleTextAttribute(hConsole, 0x80);
				printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 195, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 197, 196, 196, 196, 180);
			}
		}
		SetConsoleTextAttribute(hConsole, 0x08);
		printf("   ");
		SetConsoleTextAttribute(hConsole, 0x80);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 192, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 193, 196, 196, 196, 217);
		SetConsoleTextAttribute(hConsole, 0x08);
		printf("   ");
		for (int num = 65; num < 73; num++)
		{
			printf("  %c ", num);
		}
	}
	printf("\n\n");
}

//----------------------------------------MOVING THE PEICE FOR REAL ON THE MAIN BOARD ARRAY-------------------------------------//
void realMove(char board[BOARD_SIZE][BOARD_SIZE], int* yInitial, int* xInitial, int* yFinal, int* xFinal, bool isPlayer1)
{
	board[*yFinal][*xFinal] = board[*yInitial][*xInitial] + HIGHLIGHT_BIT; //sets the moved to highlight and moves the piece
	board[*yInitial][*xInitial] = 0 + HIGHLIGHT_BIT;	//removes the piece from the inital value and the moved from highlight piece
	printBoard(board, isPlayer1);

}

//----------------------------------------CHECKS USER INPUT FOR BOARD RANGE, OWNER OF PIECE, AND FRIENDLY PIECE IN TERMINAL LOCATION-------------------------------------//
bool userInput(char board[BOARD_SIZE][BOARD_SIZE], bool isPlayer1, int* yInitial, int* xInitial, int* yFinal, int* xFinal)
{

	char xCoord = 'B';		// [1] = { 'B' };
	char temp = 'B';			//temp character to check
	temp = tolower(temp);	//setting the temp to tolower
	bool goodMove = false;	//flag for a good move

	*xInitial = 0;				//xstart passed by reference
	*yInitial = 0;				//ystart passed by reference
	*xFinal = 0;				//xfinal passed by reference
	*yFinal = 0;				//yfinal passed by reference

	//-----------------------------------------------THIS BLOCK PROMPTS FOR USER INPUT FOR THE RIGHT USER-----------------------------------------
	do
	{
		if (isPlayer1)
		{
			printf("Player 1");
		}
		else
		{
			printf("Player 2");
		}
		printf(" enter the row for the piece to move\n");
		scanf("%d", yInitial);
	} while (*yInitial > BOARD_SIZE);

	//-----------------------------------------------THIS BLOCK PROMPTS FOR THE ALPHABETICAL ASPECT OF A MOVE-------------------------------------
	do
	{
		if (isPlayer1)
		{
			printf("Player 1");
		}
		else
		{
			printf("Player 2");
		}
		printf(" enter the letter for the piece to move\n");
		scanf(" %c", &xCoord);
		//printf("%c", xCoord);
		xCoord = toupper(xCoord);		//sets the user input to the upper case
		//printf("%c", xCoord);
		*xInitial = (int)xCoord - 65; //substracts the base case of 'A' from the letter to get a decimal value
	} while (xCoord < 'A' || xCoord > 'H');

	//----------------------------------------------THESE BLOCKS PROMPT FOR THE DESTINATION--------------------------------------------------------
	do
	{
		if (isPlayer1)
		{
			printf("Player 1");
		}
		else
		{
			printf("Player 2");
		}
		printf(" enter the row for the square to go\n");
		scanf("%d", yFinal);
	} while (*yFinal > BOARD_SIZE);

	do
	{
		if (isPlayer1)
		{
			printf("Player 1");
		}
		else
		{
			printf("Player 2");
		}
		printf(" enter the letter for the piece to go\n");
		scanf(" %c", &xCoord);
		//printf(" %c", xCoord);
		xCoord = toupper(xCoord);
		//printf(" %c", xCoord);
		*xFinal = (int)xCoord - 65;
	//	printf("xFinal = %d ", *xFinal);
	} while (xCoord < 'A' || xCoord > 'H');

	//player 2
	if (!isPlayer1)
	{
		//if its player 2, board perspective if flipped but the board never moves, these allow for easier user input
		*xInitial = 7 - *xInitial;
		*yInitial = 8 - *yInitial;
		*xFinal = 7 - *xFinal;
		*yFinal = 8 - *yFinal;

		//not moving to space with my piece
		if (!(board[*yInitial][*xInitial] & PLAYER_1_BIT) && board[*yFinal][*xFinal] == (char)0 || board[*yFinal][*xFinal] & PLAYER_1_BIT)
		{
			goodMove = true;
			//printf("that is a good move player 2\n");
		}
		else
		{
			printf("You chose to move to the square at %d, %c.  That is an invalid move player 2\n", xCoord, *yFinal);
		}
	}
	//player 1
	else
	{
		*yInitial = (*yInitial - 1);
		*yFinal = (*yFinal - 1);
		//not moving to space with my piece
		if ((board[*yInitial][*xInitial] & PLAYER_1_BIT) && (board[*yFinal][*xFinal] == (char)0 || !(board[*yFinal][*xFinal] & PLAYER_1_BIT)))
		{
			goodMove = true;
			//printf("that is a good move player 1\n");
		}
		else
		{
			printf("That is an invalid move player 1\n");
		}
	}

	return goodMove;
}

//----------------------------------------CHECKING IF THE FAKE MOVE IS A VALID MOVE-------------------------------------//
bool validMove(char board[BOARD_SIZE][BOARD_SIZE], int* yInitial, int* xInitial, int* yFinal, int* xFinal)
{
	bool canMove = false;	//flag for if piece can move

	// Check that both locations are on the board
	if (*yInitial >= BOARD_SIZE || *xInitial >= BOARD_SIZE || *yFinal >= BOARD_SIZE || *xFinal >= BOARD_SIZE)
	{
		printf("Not on the board! \n");
	}
	else if (*xInitial == *xFinal && *yInitial == *yFinal)
	{
		printf("locations are equal, you must choose a different postion! \n");
	}
	else //if any of the locations aren't within the bounds of the board
	{
		char destination = board[*yFinal][*xFinal];	// so that we do not need to keep accessing the array
		char piece = (board[*yInitial][*xInitial] & PIECE_BITS);
//		printf("the piece number is %d\n", piece);
		switch (piece)
		{
		case PAWN:	//-------------------------------------------------------------------PAWN-------------------------------------------------------------------------------------------
			if ((board[*yFinal][*xFinal] == 0) && (*xInitial == *xFinal)) //straight ahead and empty board
			{
				if (*yFinal - *yInitial == 1 || (*yInitial - *yFinal && !(board[*yInitial][*xInitial] & PLAYER_1_BIT))) // one ahead
				{
					canMove = true;
				}
				else if (((*yFinal - *yInitial == 2) && !(board[*yInitial][*xInitial] & MOVED_BIT)) || (*yInitial - *yFinal && !(board[*yInitial][*xInitial] & PLAYER_1_BIT))) // first move, two ahead
				{

					canMove = true;
				}
				else
				{
					printf("Move not possible for a pawn \n");
				}
			}
			else if ((board[*yFinal][*xFinal] != 0) && (abs(*xFinal - *xInitial) == 1) && (abs(*yFinal - *yInitial) == 1))// enemy piece, and moving one diagonal
			{
				canMove = true;
			}
			break;

		case ROOK://-------------------------------------------------------------------ROOK-------------------------------------------------------------------------------------------

			if (*xInitial == *xFinal) // rook moving vertical
			{
				if (*yInitial > *yFinal) // rook moving backward
				{
					canMove = true;
					for (int y = *yInitial - 1; y > *yFinal; y--)
					{
						if (board[y][*xFinal] != 0)
						{
							canMove = false;
						}
					}
				}
				else // rook moving forward
				{
					canMove = true;
					for (int y = *yInitial + 1; y < *yFinal; y++)
					{
						if (board[y][*xFinal] != 0)
						{
							canMove = false;
						}
					}
				}
			}
			else if (*yInitial == *yFinal) // rook moving horizontal
			{
				if (*xInitial > *xFinal) // rook moving left
				{
					canMove = true;
					for (int x = *xInitial - 1; x > *xFinal; x--)
					{
						if (board[*yFinal][x] != 0)
						{
							canMove = false;
						}
					}
				}
				else // rook moving right
				{
					canMove = true;
					for (int x = *xInitial + 1; x < *xFinal; x++)
					{
						if (board[*yInitial][x] != 0)
						{
							canMove = false;
						}
					}
				}

			}
		//	printf("the rook is able to move correctly %d\n", canMove);
			break;

		case NIGHT://-------------------------------------------------------------------KNIGHT-------------------------------------------------------------------------------------------
			if ((*xInitial - *xFinal == 1 || *xFinal - *xInitial == 1) && (*yInitial - *yFinal == 2 || *yFinal - *yInitial == 2))	//if rise is 2 and run is 1
			{
				canMove = true;
			}
			else if ((*xInitial - *xFinal == 2 || *xFinal - *xInitial == 2) && (*yInitial - *yFinal == 1 || *yFinal - *yInitial == 1))	//if rise is 1 and run is 2
			{
				canMove = true;
			}
		//	printf("the knight is able to move correctly %d\n", canMove);
			break;

		case BISHOP://-------------------------------------------------------------------BISHOP-------------------------------------------------------------------------------------------
			if (abs(*yInitial - *yFinal) == abs(*xInitial - *xFinal) && (*yInitial != *yFinal || *xInitial != *xFinal))	//can't move to the same square its in, and not in the slope of 1 or -1
			{
				if (*yInitial > *yFinal && *xInitial > *xFinal)	//moving in the -,- direction
				{
					int z = *xInitial - 1;
					int i = *yInitial - 1;
					while (board[i][z] == 0 && i > *yFinal)	//checks the path of the bishop
					{
						z--;
						i--;
					}
					if (i == *yFinal && z == *xFinal)	//if final values then a valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial < *yFinal && *xInitial < *xFinal)	//moving in the +,+ direction
				{
					int z = *xInitial + 1;
					int i = *yInitial + 1;
					while (board[i][z] == 0 && i < *yFinal)	//checks the path of the bishop
					{
						i++;
						z++;
					}
					if (i == *yFinal && z == *xFinal)	//if final values then a valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial > *yFinal && *xInitial < *xFinal)	//moving in the +,- direction
				{
					int i = *yInitial - 1;
					int z = *xInitial + 1;
					while (board[i][z] == 0 && i > *yFinal)	//checks the path of the bishop
					{
						i--;
						z++;
					}
					if (i == *yFinal && z == *xFinal)	//if final values then a valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial < *yFinal && *xInitial > *xFinal)	//moving in the -,+ direction
				{
					int z = *xInitial - 1;
					int i = *yInitial + 1;
					while (board[i][z] == 0 && i < *yFinal)	//checks the path of the bishop
					{
						i++;
						z--;
					}
					if (i == *yFinal && z == *xFinal)	//if final values then a valid move
					{
						canMove = true;
					}
				}
			}
//			printf("the bishop is able to move correctly %d\n", canMove);
			break;

		case QUEEN://-------------------------------------------------------------------QUEEN-------------------------------------------------------------------------------------------
			if (*yInitial - *yFinal == 0)	//moving like a rook horizontially
			{
				if (*xInitial > *xFinal)	//moving left on the x axis
				{
					int i = *xInitial - 1;
					while (board[*yInitial][i] == 0 && i > *xFinal)	//pathing for rook
					{
						i--;
					}
					if (i == *xFinal)
					{
						canMove = true;
					}
				}
				else if (*xInitial < *xFinal)	//moving right on the x axis
				{
					int i = *xInitial + 1;
					while (board[*yInitial][i] == 0 && i < *xFinal)	//pathing down the lane
					{
						i++;
					}
					if (i == *xFinal)	//if lane is clear then valid move
					{
						canMove = true;
					}
				}
			}
			else if (*xInitial - *xFinal == 0)	//moving like a rook vertically 
			{
				if (*yInitial > *yFinal)	//moving up the board
				{
					int i = *yInitial - 1;
					while (board[i][*xInitial] == 0 && i > *yFinal)	//pathing down the lane
					{
						i--;
					}
					if (i == *yFinal)	//if nothing in the path then valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial < *yFinal)	//moving down the board
				{
					int i = *yInitial + 1;
					while (board[i][*xInitial] == 0 && i < *yFinal)
					{
						i++;
					}
					if (i == *yFinal)
					{
						canMove = true;
					}
				}
			}

			if (abs(*yInitial - *yFinal) == abs(*xInitial - *xFinal))	//moving like a bishop
			{
				if (*yInitial > *yFinal && *xInitial > *xFinal)		//moving in the -,- direction
				{
					int z = *xInitial - 1;
					int i = *yInitial - 1;
					while (board[i][z] == 0 && i > *yFinal)	//pathing down the lane
					{
						z--;
						i--;
					}
					if (i == *yFinal && z == *xFinal)	// if lane is clear valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial < *yFinal && *xInitial < *xFinal)	//moving in the +,+ direction
				{
					int z = *xInitial + 1;
					int i = *yInitial + 1;
					while (board[i][z] == 0 && i < *yFinal)	//pathing down the lane
					{
						i++;
						z++;
					}
					if (i == *yFinal && z == *xFinal)	//if lane is clear valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial > *yFinal && *xInitial < *xFinal)	//moving in the +,- direction
				{
					int z = *xInitial + 1;
					int i = *yInitial - 1;
					while (board[i][z] == 0 && i > *yFinal)	//pathing down the lane
					{
						i--;
						z++;
					}
					if (i == *yFinal && z == *xFinal)	//if lane is clear valid move
					{
						canMove = true;
					}
				}
				else if (*yInitial < *yFinal && *xInitial > *xFinal)	//moving in the -,+ direction
				{
					int z = *xInitial - 1;
					int i = *yInitial + 1;
					while (board[i][z] == 0 && i < *yFinal)	//pathing down the lane
					{
						i++;
						z--;
					}
					if (i == *yFinal && z == *xFinal)	//if lane is clear valid move
					{
						canMove = true;
					}
				}
			}
	//		printf("the queen is able to move correctly %d\n", canMove);
			break;

		case KING://-------------------------------------------------------------------KING-------------------------------------------------------------------------------------------
			if (abs(*yInitial - *yFinal) == 1 || abs(*xInitial - *xFinal) == 1)	//can only move one space at a time
			{
				if (*yInitial == *yFinal)
				{
					canMove = true;
				}
				else if (*xInitial == *xFinal)
				{
					canMove = true;
				}
				else if (abs(*yInitial - *yFinal) == 1 && abs(*xInitial - *xFinal) == 1)
				{
					canMove = true;
				}
			}
//			printf("the king is able to move correctly %d\n", canMove);
			break;
		}
		return canMove;
	}
}
