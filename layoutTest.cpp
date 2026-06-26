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

void showIntroduction();
int getBoardSize();
char **createBoard(int size);
void initializeBoard(char **board, int size);
void displayBoard(char **board, int size);
void deleteBoard(char **board, int size);
bool convertInputToCoordinates(string input, int &row, int &col, int boardSize);

void showWelcomeMenu();
void saveGameState(char **board, int size, char currentPlayer, bool specialPowersActive[][MAX_SIZE]);
bool loadGameState(char **board, int &size, char &currentPlayer, bool specialPowersActive[][MAX_SIZE]);
bool fileExists(const string &filename);

void startMenu(char **board, int &boardSize, char &currentTurn);
void mainGameLoop(char **board, int boardSize, char startingTurn);


int main()
{
    showIntroduction();

    string menuChoice;
    int boardSize = MIN_SIZE;
    char currentPlayer = PLAYER_X;
    bool specialPowersActive[MAX_ROWS][MAX_SIZE] = {false};
    char **board = nullptr;

    showWelcomeMenu();
    cout << "Type 'NEW GAME' or 'CONTINUE': ";
    getline(cin, menuChoice);

    // string to uppercase
    for (size_t i = 0; i < menuChoice.length(); i++) {
        menuChoice[i] = toupper(menuChoice[i]);
    }

    if (menuChoice == "CONTINUE") {
        if (fileExists(SAVE_FILENAME)) {
            cout << "File save found. Loading state..." << endl;
            // Create max-size just in case
            board = createBoard(MAX_SIZE);

            if (loadGameState(board, boardSize, currentPlayer, specialPowersActive)) {
                cout << "Game loaded successfully!" << endl;
                displayBoard(board, boardSize);
            } else {
                cout << "Failed to read save file or corrupted data. Creating new game instead." << endl;
                deleteBoard(board, MAX_SIZE); // clear max board
                boardSize = getBoardSize();
                board = createBoard(boardSize);
                initializeBoard(board, boardSize);
                currentPlayer = PLAYER_X;
                displayBoard(board, boardSize);
            }
        } else {
            cout << "No save file. Creating new game..." << endl;
            boardSize = getBoardSize();
            board = createBoard(boardSize);
            initializeBoard(board, boardSize);
            currentPlayer = PLAYER_X;
            displayBoard(board, boardSize);
        }
    }
    else // New Game
    {
        boardSize = getBoardSize();
        board = createBoard(boardSize);
        initializeBoard(board, boardSize);
        currentPlayer = PLAYER_X;
        displayBoard(board, boardSize);
    }

    // coordinate translation
    string userInput;
    int targetRow = 0, targetCol = 0;

    for (int i = 0; i < 3; i++)
    {
        cout << endl;
        cout << "Enter a coordinate to translate (e.g. D2): ";
        cin  >> userInput;

        if (convertInputToCoordinates(userInput, targetRow, targetCol, boardSize))
        {
            cout << "Valid! '" << userInput << "' maps to -> Row: " << targetRow
                 << ", Col: " << targetCol << endl;
            cout << "Piece currently at that position: '" << board[targetRow][targetCol] << "'" << endl;
        }

        else
        {
            cout << "Invalid coordinate! It's either poorly formatted or out of bounds." << endl;
        }

        cout << endl;
    }
    saveGameState(board, boardSize, currentPlayer, specialPowersActive);

    deleteBoard(board, boardSize);



    cout << "Board displayed successfully.\n";

    return 0;
}

void showIntroduction()
{
    cout << "====================================\n";
    cout << "          Checkermania\n";
    cout << "====================================\n";
    cout << "Milestone 1: Dynamic Checker Board Display\n\n";
}

void showWelcomeMenu()
{
    cout << "========= GAME START MENU ==========\n";
    cout << " * NEW GAME\n";
    cout << " * CONTINUE\n";
    cout << "====================================\n";
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

bool fileExists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}

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
