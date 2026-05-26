#include <iostream>
#include <string>
#include <cctype>   //for toupper()

using namespace std;

// constant global
const int MAX_ROWS = 10;
const int MAX_COLS = 10;

/*
-----------------
func prototypes
-----------------
*/

bool convertInputToCoordinates(string input, int &row, int &col, int boardSize);
void initializeNewBoard(char board[MAX_ROWS][MAX_COLS], int boardSize);
void debugDisplayBoard(char board[MAX_ROWS][MAX_COLS], int boardSize); 

int main()
{
    // TEMPORARY SETUP: simulate a chosen board size (exp: 6x6, 8x8 or 10x10)
    int  testBoardSize = 6; 
    char testBoard[MAX_ROWS][MAX_COLS];

    cout << "---Testing Piece Placement Logic---" << endl;
    initializeNewBoard(testBoard, testBoardSize);
    debugDisplayBoard(testBoard, testBoardSize);

    cout << "---Testing Coordinate Translation Logic---" << endl;
    string userInput;
    int targetRow = 0, targetCol = 0;

    // simple loop to test inputs like "D2", "A3" or "b1"
    for (int i = 0; i < 3; i++)
    {
        cout << "Enter a coordinate to translate (e.g., D2): ";
        cin  >> userInput;

        if (convertInputToCoordinates(userInput, targetRow, targetCol, testBoardSize))
        {
            cout << "Valid! '" << userInput << "' maps to -> Row: " << targetRow 
                 << ", Col: " << targetCol << endl;
            cout << "Piece currently at that position: '" << testBoard[targetRow][targetCol] << "'" << endl;
        }
        
        else
        {
            cout << "Invalid coordinate! It's either poorly formatted or out of bounds." << endl;
        }
        cout << endl;
    }

    return 0;
}