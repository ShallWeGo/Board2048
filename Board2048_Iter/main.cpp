#include"board.h"
using namespace std;

int  main(){
	Board2048 * bo = new Board2048(4,4);
	bo->runGame();
	delete bo;
	return 0;
}
