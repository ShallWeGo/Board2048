#include"board.h"
using namespace std;

void *beginGame(void *arg){
	Board2048* bo = new Board2048(4,4);
	bo->runGame();
	delete bo;
}

int  main(){
	pthread_t tid_input, tid_game;
	pthread_setconcurrency(2);
	pthread_create(&tid_game,NULL,beginGame,NULL);
	pthread_create(&tid_input,NULL,getInput,NULL);
	/* Here means the main thread has break out to quit */
	pthread_join(tid_game,NULL);
	cout<<"Good Bye...."<<endl;
	/* game should be firstly ended */
	pthread_cancel(tid_input);
	pthread_join(tid_input,NULL);
	return 0;
}
