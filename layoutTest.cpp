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
