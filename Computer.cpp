#include <iostream>

#include "Computer.h"
#include "Board.h"
#include <vector>

using namespace std;

//megtalálja a következõ lépést az alfa-béta segítségével
//maximalizálni akarják az értékelési funkciót. Ez jó a számítógép számára
vector<int> Computer::nextMoveAlphaBeta(Board & board){
	vector<int> v = {0,0};
	int M = INT_MIN, x = -1, y = -1;

	for (int i = 0; i <= N-1;i++){
		for (int j = 0; j <= N-1; j++){
			if (board.getValue(i, j) == '.' && adjacentPlaced(board, i, j) ){
				int temp = minimaxAlphaBeta (board, 2, true,INT_MIN, INT_MAX, i, j);
				if (M < temp){
					M = temp;
					x = i;
					y = j;
				}

			}
		}
	}
	if (x == -1 && y == -1){
		x = N/2;
		y = N/2;
	}
	v[0] = x;
	v[1] = y;
	return v;
}

//A minimaxAlphaBeta alfa-béta algoritmus a számítógép következõ lépésének meghatározásáhoza
//Használja itt az értékelési funkciót a mélységgel.
//a magasabb pontszám jó a számítógép számára, az alacsonyabb a játékos számára
//isMax = True, ha az (x, y) pontnál a számítógép mozog
int Computer::minimaxAlphaBeta(Board & board, int depth, bool isMax, int alpha, int beta, int x , int y){
	char c = isMax ?  board.getComputerSymbol()  :  board.getPlayerSymbol();
	board.setValue(x,y,c);

	if (board.win(x,y)){
		board.setValue(x,y,'.');
		if (isMax){//gép nyert
			return INT_MAX;
		}else {
			return INT_MIN;//A játékos nyer, ezért minimalizálni kell a pontszámot, így a számítógép nem választja ezt az utat
		}
	}else if (board.draw()){
		board.setValue(x,y,'.');
		return 0;
	}
	if (depth == 0){
		int value = 0;
		if (checkVisitedBoard(board) == true){
			value = getEvaluation(board);
		}else{
			value = evaluation(board, isMax); //új értéket számol
			insertToHashMap(board, value); //eltárolja az új értéket a hashmap-be
		}
		board.setValue(x,y,'.');
		return value;
	}

	//itt elmenti az új elérhetõ cella x koordinátát a firstCoord-ba, az y koordinátát a secondCoord-ba
	vector<int> firstCoord;
	vector<int> secondCoord;
	for (int i = 0; i < N; i++){
		for (int j = 0; j<N; j++){
			if (board.getValue(i,j) == '.' && adjacentPlaced(board, i, j)){
				firstCoord.push_back(i);
				secondCoord.push_back(j);
			}
		}
	}

	int len = (int) firstCoord.size();
	if (isMax == true){ //probálja minimalizálni mert ez a játékos köre
		int m = INT_MAX;
		for (int i = 0; i < len; i++){
			int temp = minimaxAlphaBeta(board,depth - 1, false, alpha, beta, firstCoord[i], secondCoord[i]);
			if (m > temp){
				m = temp;
			}
			if (beta > m){
				beta = m;
			 }
			 if (alpha >= beta){
				break;
			 }
		}
		board.setValue(x,y,'.');

		return m;
	}else {//ha nem akkor maximalizálja
		int M = INT_MIN;
		for (int i = 0; i < len; i++){
			int temp = minimaxAlphaBeta(board, depth - 1, true, alpha, beta, firstCoord[i], secondCoord[i]);
			if (M < temp){
				M = temp;
			}
			if (alpha < M){
				alpha = M;
			 }
			 if (alpha >= beta){
				break;
			 }
		}
		board.setValue(x,y,'.');

		return M;
	}
}


int Computer::evaluation(Board & board, bool isMax){// hogyha isMax True, akkor a gép lépni akar az (x,y) pozicióra

	int sum = 0;
	vector<int> computerPattern(M+1,0);
	vector<int> playerPattern(M+1,0);
	for (int  i = 0 ; i < N; i++){
		for (int j = 0; j < N ; j++){
			if (board.getValue(i,j) != '.'){
				//mintákat számol oszlopokra
				char c = board.getValue(i,j);
				bool needMax = c == board.getComputerSymbol();
				int sameSymbol = 1; //az adott symbol-ra kiszámolja a sort
				int k = 1;
				while (i- k >= 0 && board.getValue(i-k, j)  == c){
					sameSymbol++;
					k++;
				}
				//megnézi hogy az i-k-dik pozició blockolt-e vagy nem
				int l = 1;
				while (i + l <= N-1 && board.getValue(i+l, j) == c){
					sameSymbol++;
					l++;
				}
				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i-k,j) || board.checkEmpty(i+l,j))){
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j) || board.checkEmpty(i+l,j))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j) && board.checkEmpty(i+l,j))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i-k,j) && board.checkEmpty(i+l,j)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				sameSymbol = 1;//az adott symbol - ra kiszámolja az oszlopot
				k = 1;
				while (j - k >= 0 && board.getValue(i, j-k)  == c){
					sameSymbol++;
					k++;
				}
				//megnézi hogy az i-k-dik pozició blockolt-e vagy nem
				l = 1;
				while (j + l <= N-1 && board.getValue(i, j+l) == c){
					sameSymbol++;
					l++;
				}

				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i,j-k) || board.checkEmpty(i,j+l))){
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i,j-k) || board.checkEmpty(i,j+l))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i,j-k) && board.checkEmpty(i,j+l))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i,j-k) && board.checkEmpty(i,j+l)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				sameSymbol = 1;// az adott symbol-ra kiszámolja a fõ diagnózist
				k = 1;
				while (i - k >= 0 && j - k >= 0 && board.getValue(i-k, j- k)  == c){
					sameSymbol++;
					k++;
				}


				//megnézi hogy az i-k-dik pozició blockolt-e vagy nem
				l = 1;
				while (i + l <= N-1 && j + l <= N-1 && board.getValue(i+l, j+l ) == c){
					sameSymbol++;
					l++;
				}

				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i-k,j-k) || board.checkEmpty(i+l,j+l))){
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j-k) || board.checkEmpty(i+l,j+l))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j-k) && board.checkEmpty(i+l,j+l))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i-k,j-k) && board.checkEmpty(i+l,j+l)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				sameSymbol = 1;// az adott symbol-ra kiszámolja a fordított diagnózist
				k = 1;
				while (i - k >= 0 && j + k <= N-1 && board.getValue(i-k, j+ k)  == c){
					sameSymbol++;
					k++;
				}
				//megnézi hogy az i-k-dik pozició blockolt-e vagy nem
				l = 1;
				while (i + l <= N-1 && j - l >= 0 && board.getValue(i+l, j-l ) == c){
					sameSymbol++;
					l++;
				}
				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i-k,j+k) || board.checkEmpty(i+l,j-l))){
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j+k) || board.checkEmpty(i+l,j-l))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j+k) && board.checkEmpty(i+l,j-l))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i-k,j+k) && board.checkEmpty(i+l,j-l)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}


			}
		}
	}
	if (computerPattern[M] > 0) return INT_MAX;
	if (playerPattern[M] > 0) return INT_MIN;
	int x = 1;
	sum += computerPattern[1];
	sum -= playerPattern[1]*5;
	for (int i = 2 ; i < M ; i++){
		x *= 100;
		sum += computerPattern[i] * x;
		sum -= playerPattern[i] * x * 10;
	}
	return sum;
}




bool Computer::adjacentPlaced(Board & board , int x, int y){

	bool value = false;
	if (board.getValue(x,y) != '.') return false;
	vector<vector<int>> adjacent = {{-1,-1},{-1,0}, {-1,1},{0,1},{0,-1},{1,-1},{1,0},{1,1}};
	for (auto d:adjacent){

		if (x+d[0] >=0 && y+d[1]>=0 && x+d[0] <= N-1 && y + d[1] <= N-1){
			value = value || (board.getValue(x+d[0],y+d[1]) != '.');
		}
	}
	return value;
}


//ellenõrzi, hogy az adott tábla kiértékelési funkciója már van-e a memóriában vagy sem
bool Computer::checkVisitedBoard(Board & board){
	string s = board.toString();
	if (hashMap.find(s) != hashMap.end()){
		return true;
	}
	return false;
}


// ha az adott tábla benne van akkor csak ki kell vennie
// ez idõt fog megtakarítani
int Computer::getEvaluation(Board & board){
	if (checkVisitedBoard(board)){
		return hashMap[board.toString()];
	}
	return -1;
}

//beszúrja az értéket a hashmap-be
void Computer::insertToHashMap(Board& board, int eval) {
	string s = board.toString();
	hashMap[s] = eval;
	return;
}
