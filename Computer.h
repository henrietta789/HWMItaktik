#ifndef COMPUTER_H_INCLUDED
#define COMPUTER_H_INCLUDED


#include <stdio.h>
#include <iostream>
#include <vector>
#include "Board.h"
#include <vector>
#include <unordered_map>

using namespace std;

class Computer{
	private:
	unordered_map<string, int> hashMap;

	public:
	vector<int> nextMove(Board &) ;
	vector<int> nextMoveAlphaBeta(Board &) ;
	int minimax(Board &, int, bool, int, int) ;
	int minimaxAlphaBeta(Board &, int, bool, int, int , int , int );
	int evaluation (Board &, bool);
	bool adjacentPlaced(Board &, int, int);
	bool checkVisitedBoard(Board &);
	int getEvaluation(Board & );
	void insertToHashMap(Board &, int);
};

#endif // COMPUTER_H_INCLUDED
