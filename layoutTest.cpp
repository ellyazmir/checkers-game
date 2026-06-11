#include <iostream>
#include <string>
#include <cctype>   //for toupper()
#include <fstream>

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

bool checkSaveFile()
bool loadGame(char board[MAX_ROWS][MAX_COLS], int &boardSize, char &currentTurn );
void startMenu (char board[MAX_ROWS][MAX_COLS], int &boardSize, char &currentTurn);
void mainGameLoop (char board[MAX_ROWS][MAX_COLS], int boardSize, char startingTurn);

int main()
{
 /*
---------------------------------------------------------------------------------
DEVELOPER: Trisha Adelina Binti Shuhaini
TASK     : Start Menu and Save/Load Logic
---------------------------------------------------------------------------------
*/

    string menuChoice;
    int activeBoardSize = 6; // default size for game state
    char activeBoard[MAX_ROWS][MAX_COLS];

    cout << "Type 'NEW GAME' or 'CONTINUE': ";
    getline(cin, menuChoice);

    // convert to uppercase
    for (int i = 0; i < menuChoice.length(); i++) {
        menuChoice[i] = toupper(menuChoice[i]);
    }

    if (menuChoice = "CONTINUE"){
        if (checkSaveFile()){
            cout >> "File save found. Loading state" << endl;
            loadGame(activeBoard, activeBoardSize);
        } else {
        cout >> "No save file. Creating new" << endl;
        initializeNewBoard(activeBoard, activeBoardSize);
        }
    } else {
    // default new game
    initializeNewBoard(activeBoard, activeBoardSize);
    }






    // TEMPORARY SETUP: simulate a chosen board size (exp: 6x6, 8x8 or 10x10)
    int  testBoardSize = 6;
    char testBoard[MAX_ROWS][MAX_COLS];

    cout << "---Testing Piece Placement Logic---" << endl;
    initializeNewBoard(testBoard, testBoardSize);
    debugDisplayBoard(testBoard, testBoardSize);

    cout << endl << endl;
    cout << "---Testing Coordinate Translation Logic---" << endl;
    string userInput;
    int targetRow = 0, targetCol = 0;

    // simple loop to test inputs like "D2", "A3" or "b1"
    for (int i = 0; i < 3; i++)
    {
        cout << endl;
        cout << "Enter a coordinate to translate (exp: D2): ";
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

------------------------------------------------------------------------------
DEVELOPER: ELLY MAZLIN
TASK     : auto place 'O' & 'X' pieces onto valid alternating dark diagonals
------------------------------------------------------------------------------
*/

void initializeNewBoard(char board[MAX_ROWS][MAX_COLS], int boardSize)
{
    // 1. clear entire array container safely with empty spaces
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLS; j++)
        {
            board[i][j] = ' ';
        }
    }

    // 2. loop through the active board boundaries to place initial checker pieces
    for (int r = 0; r < boardSize; r++)
    {
        for (int c = 0; c < boardSize; c++)
        {
            // remainder evaluation: checker pieces only sit where row + col is odd
            if ((r + c) % 2 == 1)
            {
                // P1 ('O') occupy the top 2 rows of the board
                if (r == 0 || r == 1)
                {
                    board[r][c] = 'o';
                }
                // P2 ('X') occupy the bottom 2 rows of the board
                else if (r == (boardSize - 2) || r == (boardSize - 1))
                {
                    board[r][c] = 'x';
                }
            }
        }
    }
}

/*
-----------------------------------------
DEVELOPER: ELLY MAZLIN
TASK     : temporary test grid display
NOTE     : to see if the formulas work
-----------------------------------------
*/

void debugDisplayBoard(char board[MAX_ROWS][MAX_COLS], int boardSize)
{
    cout << endl;

    // 1. print top margin spacing to align with the row letters
    cout << "    ";

    // 2. loop to print column headers (emp:  1   2   3   4...)
    for (int c = 1; c <= boardSize; c++)
    {
        cout << c << "   "; // print num followed by 3 spaces
    }
    cout << endl;

    // 3. loop through rows
    for (int r = 0; r < boardSize; r++)
    {
        char rowLetter = 'A' + r;
        cout << rowLetter << "  "; // letter followed by 2 spaces to align with grid

        // 4. loop through columns to print the actual board slots
        for (int c = 0; c < boardSize; c++)
        {
            cout << "[" << board[r][c] << "] ";
        }

        cout << endl;
    }
}




/*
------------------------------------------------------------------------------
DEVELOPER: Trisha Adelina Binti Shuhaini
TASK     : save/load to  see if save file exist
------------------------------------------------------------------------------
*/

bool checkSaveFile()
{
    ifstream file ("savegame.txt");
    return file.good();
}

/*
---------------------------------------------------------------------------------
DEVELOPER: Trisha Adelina Binti Shuhaini
TASK     : extract data from save/load
---------------------------------------------------------------------------------
*/
void loadGame(char board[MAX_ROWS][MAX_COLS], int &boardSize)
{
    ifstream file("savegame.txt");
    if (!file.is_open()) return;

    file >> boardSize;
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++){
            file >> board [r][c]
            if (board[r][c] == '_'){
                 board[r][c] = ' ';
        }
      }

    }
    file.close();
}
