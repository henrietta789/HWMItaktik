
#include <iostream>
#include "Game.h"
#include "Board.h"
#include "Computer.h"
#include <vector>

using namespace std;

Game::Game()
{
    board.clear();
}

void Game::playConsole()
{
    board.clear();
    Computer computer;
    bool flag = true;
    int current = 1;
    int x = -1, y = -1;
    vector<int> v;
    int playerChoice;
    char XorO;

    cout << "Choose: X or O!"<<endl;
    cin >> XorO;
    if(XorO=='X' || XorO=='x')
        playerChoice=1;
    if(XorO=='o' || XorO=='O')
        playerChoice=1;

    if (playerChoice == 1)
    {
        board.setPlayerSymbol('X');
        board.setComputerSymbol('O');
    }
    else
    {
        board.setPlayerSymbol('O');
        board.setComputerSymbol('X');
    }
    cout << "Your symbol: " << board.getPlayerSymbol() <<"."<<endl;
    cout << "You want to start? y/n"<<endl;
    char playerGoFirst;
    cin >> playerGoFirst;

    if (playerGoFirst == 'Y' || playerGoFirst == 'y')
    {
        current = 1;
        cout << "Oke, you start!"<<endl;
    }
    else
    {
        cout << "Oke, I start!"<<endl;
        current = 2;
    }
    board.print();
    while (flag)
    {
        cout <<endl;
        if (current == 1)  //játékos mozog
        {
            cout <<"Your turn. (" << getCurrentSymbol(current)<<" )"<<endl;
            cout << "Enter X coordinate of your move: ";
            cin >> x;
            cout << "Enter Y coordinate of your move: ";
            cin >> y;
        }
        else
        {
            v = computer.nextMoveAlphaBeta(board);
            x = v[0];
            y = v[1];
        }

        //lépés jó vagy nem
        if (!board.checkEmpty(x, y))
        {
            cout <<endl<< "Hibas lepes, probald ujra!"<<endl;
            continue;
        }
        else //update
        {
            board.setValue(x, y, getCurrentSymbol(current));
            board.print();
            if (current == 1)
            {
                cout << "Your move : (" << x << ", " << y<<") "<<endl;
            }
            else
            {
                cout <<	"My move : (" << x << ", " << y<<") "<<endl;
            }
        }
        //játék vége vagy nem
        if (board.win(x, y))
        {
            board.print();
            if (current == 1)
            {
                cout << "You WIN!"<<endl;
            }
            else
            {
                cout << "I WON!"<<endl;
            }
            break;
        }
        else if (board.draw())
        {
            board.print();
            cout << "Draw!"<<endl;
            break;
        }
        else//játékos választás
        {
            current = 3 - current;
        }
    }
}

//aktuális játékos szimbóluma
char Game::getCurrentSymbol(int current)
{
    if (current == 1)
    {
        return board.getPlayerSymbol();
    }
    else
    {
        return board.getComputerSymbol();
    }
}








//start the game
void Game::run()
{
    bool flag = true;
    char x;
    while (flag)
    {
        playConsole();
        cout << "Szeretnel jatszani meg? y/n"<<endl;
        cin >> x;
        if (x == 'N' || x == 'n')
        {
            flag = false;
        }
        else
        {
            cout << "------------------------------------------------------"<<endl;
        }
    }
}

