/*
Program: checkermania.cpp
Course: CCP6114 Programming Fundamentals
Lecture Class: TC2L 
Tutorial Class: TT7L 
Trimester: 2610
Information: 252UC241RN ELLY MAZLIN BINTI MOHD AZMIR  ELLY.MAZLIN.MOHD1@student.mmu.edu.my        0126623767
             261UC250J4 BRYAN WONG KAI JIE            BRYAN.WONG.KAI1@student.mmu.edu.my          0143331772
             261UC2432G LIM GUAN XU                   LIM.GUAN.XU2@student.mmu.edu.my             0165151547
             252UC241Q6 TRISHA ADELINA BINTI SHUHAINI TRISHA.ADELINA.SHUHAINI1@student.mmu.edu.my 0196458508 
*/

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <cctype>   //for toupper()
#include <fstream>

using namespace std;

const int MIN_SIZE = 6;
const int MAX_SIZE = 10;
const int MAX_ROWS = MAX_SIZE;
const int MAX_COLS = MAX_SIZE;
const char EMPTY    = ' ';
const char PLAYER_X = 'x';
const char PLAYER_O = 'o';
const string SAVE_FILENAME = "savegame.txt";

// global tracking for special powers
bool shieldedPieces[MAX_SIZE][MAX_SIZE] = {false};

void showIntroduction();
int getBoardSize();
char **createBoard(int size);
void initializeBoard(char **board, int size);
void displayBoard(char **board, int size);
void deleteBoard(char **board, int size);
bool convertInputToCoordinates(string input, int &row, int &col, int boardSize);

void switchTurn(char &currentPlayer);
bool isGameOver(char **board, int size, char currentPlayer);
int countPieces(char **board, int size, char player);
void getMoveInput(string &from, string &to, char currentPlayer);

void showWelcomeMenu();
void saveGameState(char **board, int size, char currentPlayer, bool specialPowersActive[][MAX_SIZE]);
bool loadGameState(char **board, int &size, char &currentPlayer, bool specialPowersActive[][MAX_SIZE]);
bool fileExists(const string &filename);
void startMenu(char **board, int &boardSize, char &currentTurn);
void mainGameLoop(char **board, int boardSize, char startingTurn);

bool checkForPromotion(char **board, int size, int row, int col, char player);
void specialPowerMenu(int &choice);
void redHawk(char **board, int size, int row, int col, char player);
void flowState(char **board, int size, int row, int col);
void shambles(char **board, int size, int row, int col, char player);

int main()
{
    bool specialPowersActive[MAX_SIZE][MAX_SIZE] = {false};
    char **board = nullptr;
    int boardSize = 0;
    char currentPlayer = PLAYER_X;

    showIntroduction();
    showWelcomeMenu();

    cin.ignore();

    // Get user choice
    string menuChoice;
    cout << "Type 'NEW GAME' or 'CONTINUE': ";
    getline(cin, menuChoice);

    // string to uppercase
    for (size_t i = 0; i < menuChoice.length(); i++)
    {
        menuChoice[i] = toupper(menuChoice[i]);
    }

    if (menuChoice == "CONTINUE")
    {
        if (fileExists(SAVE_FILENAME))
        {
            cout << "File save found. Loading state..." << endl;
            // Create max-size just in case
            board = createBoard(MAX_SIZE);
            
            if (loadGameState(board, boardSize, currentPlayer, specialPowersActive))
            {
                cout << "Game loaded successfully!" << endl;
                displayBoard(board, boardSize);
            } else {
                cout << "Failed to read save file or corrupted data. Creating new game instead." << endl;
                deleteBoard(board, MAX_SIZE);  // clean max board
                boardSize = getBoardSize();
                board = createBoard(boardSize);
                initializeBoard(board, boardSize);
                currentPlayer = PLAYER_X;
                displayBoard(board, boardSize);
            }
        } else {
            cout << "No save file found. Creating new game..." << endl;
            boardSize = getBoardSize();
            board = createBoard(boardSize);
            initializeBoard(board, boardSize);
            currentPlayer = PLAYER_X;
            displayBoard(board, boardSize);
        }
    }
    else // NEW GAME
    {
        boardSize = getBoardSize();
        board = createBoard(boardSize);
        initializeBoard(board, boardSize);
        currentPlayer = PLAYER_X;
        displayBoard(board, boardSize);
    }
    
    // game setup
    while (!isGameOver(board, boardSize, currentPlayer))
    {
        // player move input
        string from;
        string to;
        getMoveInput(from, to, currentPlayer);

        // coordinate conversion
        int fromRow = 0, fromCol = 0;
        int toRow = 0, toCol = 0;
        bool validFrom = convertInputToCoordinates(from, fromRow, fromCol, boardSize);
        bool validTo = convertInputToCoordinates(to, toRow, toCol, boardSize);

        // coordinate validation
        if (!validFrom || !validTo)
        {
            cout << "Invalid coordinates!" << endl;
            continue;
        }

        // guanxu: move validation & capture logic here

        // board update
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = EMPTY;

        // special power check
        if (checkForPromotion(board, boardSize, toRow, toCol, currentPlayer))
        {
            int powerChoice;
            specialPowerMenu(powerChoice);

            switch (powerChoice)
            {
                case 1:
                    redHawk(board, boardSize, toRow, toCol, currentPlayer);
                    break;
                case 2:
                    flowState(board, boardSize, toRow, toCol);
                    break;
                case 3:
                    shambles(board, boardSize, toRow, toCol, currentPlayer);
                    break;
                default:
                    cout << "Invalid choice! No power activated..." << endl;
                    break;
            }

            // update specialPowersActive for save/load
            specialPowersActive[toRow][toCol] = true;
        }

        // board display
        displayBoard(board, boardSize);

        saveGameState(board, boardSize, currentPlayer, specialPowersActive);

        // player turn switching
        switchTurn(currentPlayer);
    }

    saveGameState(board, boardSize, currentPlayer, specialPowersActive);

    // memory cleanup
    deleteBoard(board, boardSize);
    cout << "Game Over!" << endl;
    return 0;
}

void showIntroduction()
{
    cout << "====================================\n";
    cout << "          Checkermania\n";
    cout << "====================================\n";
    cout << "Milestone 1: Dynamic Checker Board Display\n\n";
}

int getBoardSize()
{
    int size;
    bool validSize = false;

    do
    {
        cout << "Enter board size (" << MIN_SIZE << " to " << MAX_SIZE << "): ";
        cin >> size;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            size = 0;
        }

        if (size >= MIN_SIZE && size <= MAX_SIZE)
        {
            validSize = true;
        }
        else
        {
            cout << "Invalid input. Please enter a number from "
                 << MIN_SIZE << " to " << MAX_SIZE << ".\n\n";
        }
    } while (!validSize);

    return size;
}

char **createBoard(int size)
{
    char **board = new char *[size];

    for (int row = 0; row < size; row++)
    {
        board[row] = new char[size];

        for (int col = 0; col < size; col++)
        {
            board[row][col] = EMPTY;
        }
    }

    return board;
}

void initializeBoard(char **board, int size)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if ((row + col) % 2 == 1)
            {
                if (row < 2)
                {
                    board[row][col] = PLAYER_O;
                }
                else if (row >= size - 2)
                {
                    board[row][col] = PLAYER_X;
                }
            }
        }
    }
}

void displayBoard(char **board, int size)
{
    cout << "\n";

    for (int row = 0; row < size; row++)
    {
        cout << "   ";

        for (int col = 0; col < size; col++)
        {
            cout << "|---";
        }

        cout << "|\n";
        cout << "   ";

        for (int col = 0; col < size; col++)
        {
            cout << "| " << board[row][col] << " ";
        }

        cout << "|  " << static_cast<char>('A' + row) << "\n";
    }

    cout << "   ";

    for (int col = 0; col < size; col++)
    {
        cout << "|---";
    }

    cout << "|\n";
    cout << "     ";

    for (int col = 1; col <= size; col++)
    {
        cout << left << setw(4) << col;
    }

    cout << right << "\n\n";
}

void deleteBoard(char **board, int size)
{
    for (int row = 0; row < size; row++)
    {
        delete[] board[row];
    }

    delete[] board;
}

bool convertInputToCoordinates(string input, int &row, int &col, int boardSize)
{
    // validation: ensure the string has exactly a row character & column number (length = 2)
    if (input.length() != 2)
        return false;

    // convert char case using toupper
    char letter = toupper(input[0]);
    row = letter - 'A';

    char numChar = input[1];
    col = numChar - '1';

    // ensure the coordinates exist on the active board size
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
        return false;

    return true;
}

void switchTurn(char &currentPlayer)
{
    if (currentPlayer == PLAYER_X)
    {
        currentPlayer = PLAYER_O;
    }
    else
    {
        currentPlayer = PLAYER_X;
    }
}

// counts the remaining pieces for a player
int countPieces(char **board, int size, char player)
{
    int count = 0;
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if (board[row][col] == player)
            {
                count++;
            }
        }
    }
    return count;
}

// obtains move coordinates from the current player
void getMoveInput(string &from, string &to, char currentPlayer)
{
    cout << endl;
    cout << "Player " << currentPlayer << "'s turn" << endl;
    cout << "From: ";
    cin  >> from;
    cout << "To: ";
    cin  >> to;
}

// this checks if the game has ended
bool isGameOver(char **board, int size, char currentPlayer)
{
    int xPieces = countPieces(board, size, PLAYER_X);
    int oPieces = countPieces(board, size, PLAYER_O);

    if (xPieces == 0) // if player x runs out of pieces player o will win
    {
        cout << "Player O wins!" << endl;
        return true;
    }

    if (oPieces == 0) // if player o runs out of pieces player o will win
    {
        cout << "Player X wins!" << endl;
        return true;
    }

    return false;
}

// WELCOME MENU
void showWelcomeMenu()
{
    cout << "========= GAME START MENU ==========\n";
    cout << " * NEW GAME\n";
    cout << " * CONTINUE\n";
    cout << "====================================\n";
}

bool fileExists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}

// SAVE AND LOAD FUNCTIONS
void saveGameState(char **board, int size, char currentPlayer, bool specialPowersActive[][MAX_SIZE])
{
    ofstream outFile(SAVE_FILENAME);
    if (!outFile)
    {
        cout << "Error: Could not open file for writing data storage state.\n";
        return;
    }

    // Write parameters
    outFile << size << "\n";
    outFile << currentPlayer << "\n";

    // write saved pieces using placeholders
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (board[r][c] == EMPTY) outFile << '_';
            else outFile << board[r][c];
        }
        outFile << "\n";
    }

    // tracks special power
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            outFile << specialPowersActive[r][c] << " ";
        }
        outFile << "\n";
    }

    outFile.close();
    cout << "Game progress recorded successfully.\n";
}

bool loadGameState(char **board, int &size, char &currentPlayer, bool specialPowersActive[][MAX_SIZE])
{
    ifstream inFile(SAVE_FILENAME);
    if (!inFile) return false;

    // Read general attribute
    if (!(inFile >> size >> currentPlayer)) return false;

    // Read structural map allocation
    char cellData;
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (!(inFile >> cellData)) return false;
            if (cellData == '_') board[r][c] = EMPTY;
            else board[r][c] = cellData;
        }
    }

    // read special powers used
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (!(inFile >> specialPowersActive[r][c])) return false;
        }
    }

    inFile.close();
    return true;
}

bool checkForPromotion(char **board, int size, int row, int col, char player)
{
    // P-X promotes when reaching row 0 (top - opponent's side)
    if (player == PLAYER_X && row == 0)
    {
        return true;
    }

    // P-O promotes when reaching row size-1 (bottom - opponent's side)
    if (player == PLAYER_O && row == size - 1)
    {
        return true;
    }

    return false;
}

void specialPowerMenu(int &choice)
{
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "  ⊹₊˚‧︵‿₊୨ SPECIAL POWER UNLOCKED ୧₊‿︵‧˚₊⊹  " << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    cout << "Your piece reached the enemy's side!" << endl;
    cout << "Choose a power:" << endl;
    cout << "  1. 🐦‍🔥 RED HAWK   - Jump over 2 opponent pieces" << endl;
    cout << "  2. 🔮 FLOW STATE - Become immune to capture (1 turn)" << endl;
    cout << "  3. 💥 SHAMBLES   - Capture opponent 2 spaces away" << endl;
    cout << "Your choice (1-3): ";
    cin  >> choice;
}

void redHawk(char **board, int size, int row, int col, char player)
{
    char opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    int  direction = (player == PLAYER_X) ? -2 : 2;  // X move up, O move down

    cout << "Enter destination column (1-" << size << "): ";
    int destCol;
    cin  >> destCol;
    destCol--;

    int newRow = row + direction;
    int newCol = destCol;

    // check bounds
    if (newRow < 0 || newRow >= size || newCol < 0 || newCol >= size)
    {
        cout << "Destination out of bounds!" << endl;
        return;
    }

    // check destination is empty
    if (board[newRow][newCol] != EMPTY)
    {
        cout << "Destination occupied!" << endl;
        return;
    }

    // must move 2 spaces diagonally or straight
    if (abs(col - newCol) != 2 && abs(col - newCol) != 0)
    {
        cout << "Sky Walk requires 2 spaces diagonally or straight!" << endl;
        return;
    }

    // check if there are TWO opponent pieces in between
    int midRow1 = row + (direction / 2);
    int midCol1 = (col + newCol) / 2;
    int midRow2 = row + direction;
    int midCol2 = col + (newCol - col) / 2;

    if (board[midRow1][midCol1] == opponent && board[midRow2][midCol2] == opponent)
    {
        // both are opponents -> capture both
        board[midRow1][midCol1] = EMPTY;
        board[midRow2][midCol2] = EMPTY;

        // move the piece
        board[newRow][newCol] = board[row][col];
        board[row][col] = EMPTY;

        cout << "🐦‍🔥 RED HAWK! TWO opponent pieces destroyed!" << endl;
    }
    
    else
    {
        cout << "Need TWO opponent pieces in the path!" << endl;
    }
}

void flowState(char **board, int size, int row, int col)
{
    shieldedPieces[row][col] = true;

    cout << "🔮 FLOW STATE activated! This piece is immune to capture for 1 turn." << endl;
    cout << "Your opponent cannot capture this piece next turn." << endl;
}

void shambles(char **board, int size, int row, int col, char player)
{
    char opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    int  directions[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
    bool hit = false;

    for (int i = 0; i < 4; i++)
    {
        int targetRow = row + directions[i][0];
        int targetCol = col + directions[i][1];

        if (targetRow >= 0 && targetRow < size && targetCol >= 0 && targetCol < size)
        {
            if (board[targetRow][targetCol] == opponent)
            {
                board[targetRow][targetCol] = EMPTY;
                cout << "💥 SHAMBLES! Opponent piece at "
                     << char('A' + targetRow) << (targetCol + 1)
                     << " destroyed!" << endl;
                hit = true;
            }
        }
    }

    if (!hit)
    {
        cout << "No opponent piece within sniper range!" << endl;
    }
}