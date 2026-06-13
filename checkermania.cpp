/*
Program: layoutTest.cpp
Course: CCP6114 Programming Fundamentals
Lecture Class: TC2L 
Tutorial Class: TT7L 
Trimester: 2610
Information: 252UC241RN ELLY MAZLIN BINTI MOHD AZMIR  ELLY.MAZLIN.MOHD1@student.mmu.edu.my        0126623767
             261UC205J4 BRYAN WONG KAI JIE            BRYAN.WONG.KAI1@student.mmu.edu.my          0143331772
             261UC2432G LIM GUAN XU                   LIM.GUAN.XU2@student.mmu.edu.my             0165151547
             252UC241Q6 TRISHA ADELINA BINTI SHUHAINI TRISHA.ADELINA.SHUHAINI1@student.mmu.edu.my 0196458508 
*/

#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

const int MIN_SIZE = 6;
const int MAX_SIZE = 10;
const char EMPTY = ' ';
const char PLAYER_X = 'x';
const char PLAYER_O = 'o';

void showIntroduction();
int getBoardSize();
char **createBoard(int size);
void initializeBoard(char **board, int size);
void displayBoard(char **board, int size);
void deleteBoard(char **board, int size);

int main()
{
    int boardSize;
    char **board;

    showIntroduction();
    boardSize = getBoardSize();

    board = createBoard(boardSize);
    initializeBoard(board, boardSize);
    displayBoard(board, boardSize);
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
