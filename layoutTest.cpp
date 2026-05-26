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

/*
---------------------------------------------------------------------------------
DEVELOPER: ELLY MAZLIN
TASK     : translate coordinate strings (exp: "D2") into valid 0-indexed values
---------------------------------------------------------------------------------
*/

bool convertInputToCoordinates(string input, int &row, int &col, int boardSize)
{
    // validation: ensure the string has exactly a row character & column number (length = 2)
    if (input.length() != 2)
    {
        return false;
    }
    
    // convert char case using toupper from <cctype>
    char letter = toupper(input[0]); 
    row = letter - 'A'; // math char mapping ('A' becomes 0, 'B' becomes 1...)
    
    char numChar = input[1];
    col = numChar - '1'; // math char mapping ('1' becomes 0, '2' becomes 1...)
    
    // boundary assessment: ensure the coordinates exist on the active board size
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
    {
        return false;
    }
    
    return true; 
}