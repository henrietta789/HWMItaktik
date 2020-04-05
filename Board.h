#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <stdio.h>

using namespace std;

const int N = 15;
const int M = 5;

class Board
{
private:
    char board[N][N];
    char playerSym;
    char computerSym;
    int count;
public:
    Board();
    void clear();
    void print();
    bool checkEmpty(int, int);
    bool win(int, int);
    bool draw();
    char getValue(int, int);
    void setValue(int, int, char);
    int getCount();
    char getPlayerSymbol()
    {
        return playerSym;
    }
    char getComputerSymbol()
    {
        return computerSym;
    }
    void setPlayerSymbol(char c)
    {
        playerSym = c;
    }
    void setComputerSymbol(char c)
    {
        computerSym = c;
    }
    bool rowOfFive(int, int);
    bool columnOfFive(int, int);
    bool mainDiagnolOfFive(int, int);
    bool reverseDiagnolOfFive(int, int);
    string toString();
};

#endif // BOARD_H_INCLUDED
