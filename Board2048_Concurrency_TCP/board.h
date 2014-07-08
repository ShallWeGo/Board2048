#ifndef _BOARD_H
#define _BOARD_H

#include<iostream>
#include<vector>
#include<termios.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define LOGO "Game 2048: \n<Author ShallWeGo> <Date 6.15.2014> <Version 0.1> <email:wtwbupt@gmail.com>\nU--Up D--Down L--Left R--Right Q--ESC"
using namespace std;
int getch();

struct share{
        pthread_mutex_t mutex;
        pthread_cond_t  cond;
        int             nready;
        char            Way;
};
extern share shared;
void* getInput(void *arg);
int getch();

class Point{
public:
	int x;
	int y;
	Point(int i,int j):x(i),y(j){}
	Point(const Point& rhs){
		x = rhs.x;
		y = rhs.y;
	}
	Point& operator=(Point rhs){
		if(rhs.x == x && rhs.y == y) return *this;
		x = rhs.x;
		y = rhs.y;
		return *this;
	}	
};

class Board2048{
private:
	vector< vector<int> >	board;
	int			keyNum;
	int			maxKey;
	int			score;
	int			b_row;
	int			b_col;
	char			b_way;
	char			b_quit;
public:
	/* Constructor */
	Board2048(int row, int col):b_row(row),b_col(col),keyNum(0),maxKey(0),score(0),b_quit(' '){
		board = vector< vector<int> >(b_row, vector<int>(b_col,0));
	}
	/* begin the game */
	void 	runGame();
	int 	getRow();
	int	getCol();
	int	getWay();
	/* showBoard */	
	void	showBoard(char);
	bool	randKey(int);
	Point 	randPoint();
	int 	getKeyNum();
	/* moveBoard */
	void	moveBoard();
	bool	combineKey(int,int,int);
	int	getMaxKey();
	/* checkOver */
	void	checkOver();
	int	getScore();
	int	countScore();
	/* get input*/
	void	getInput();
	int	getch();
};

#endif
