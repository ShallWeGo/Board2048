#include"board.h"
#include<exception>
#include<iomanip>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<map>
share shared = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
#define SERV_PORT	9877
#define BACKLOG 10
#define MAXLINE 128

void *getInput(void* arg){
			int listenfd,connfd;
			struct sockaddr_in servaddr, clientaddr;
			socklen_t clientlen;
			
			/* Server config */
			bzero(&servaddr, sizeof(servaddr));
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(SERV_PORT);
			servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0){
						perror("socket create failed!");
						exit(1);
			}
			if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
						perror("bind error!");
						exit(1);
			}
			if(listen(listenfd, BACKLOG)<0){
						perror("listen failed!");
			}
			
			/* select config */
			fd_set mset, allset;
			int maxfd,n;
			char readbuf[MAXLINE+1];
			map<int, struct sockaddr_in*> nmap;
			FD_ZERO(&mset);
			FD_ZERO(&allset);
			maxfd = listenfd;
			FD_SET(listenfd, &allset);
			
			/* monitor the input client and new client */
			for(;;){
						mset = allset;
						select(maxfd+1, &mset, NULL, NULL, NULL);
						if(FD_ISSET(listenfd, &mset)){
								connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
								if(connfd>maxfd)
									maxfd = connfd;
								nmap[connfd] = &clientaddr;
								FD_SET(connfd, &allset);
								continue;
						}
						for(map<int, struct sockaddr_in*>::iterator iter = nmap.begin();iter!=nmap.end();iter++){
									int tempfd = iter->first;
									if(!FD_ISSET(tempfd, &mset) ){ continue;}
									else if ((n = recv(tempfd, readbuf, MAXLINE,0))!=0){
												pthread_mutex_lock(&shared.mutex);
												shared.Way = (char)(readbuf[0]);
												if(shared.nready==0)
	                       		pthread_cond_signal(&shared.cond);
	                			shared.nready++;
	                			pthread_mutex_unlock(&shared.mutex);
	                			//usleep(1000);//有了select，应该可以不用sleep了
	                			break;
			            }		
				   else{
						printf("nothing recved! %d \n", iter->first);
						break;
					}
					
						}
			}
					
}


int Board2048::getRow(){
	return b_row;
}

int Board2048::getCol(){
	return b_col;
}

int Board2048::getMaxKey(){
	return maxKey;
}

int Board2048::getScore(){
	return score;
}

int Board2048::getKeyNum(){
	return keyNum;
}

int Board2048::getWay(){
	return b_way;
}

int Board2048::countScore(){
}

void Board2048::runGame(){
	srand(time(0));
try
{
	/* check board */
	if(board.size()==0){
		throw exception();
	}
	/* rand a key to put on the board */
	randKey(2);
	/* Show the initial board */
	assert(getKeyNum()==2);
	showBoard('N');
	while(1){
		/* get input char */
		getInput();
		if(b_quit == 'q')
		  	break;
		else if(b_quit=='n'){
			b_quit = ' ';
			continue;
		}
		moveBoard();
		randKey(1);
		showBoard('N');
		checkOver();
		if(b_quit == 'q')
			break;
	}
	cout<<endl;
}catch(exception e){
	cout<<"Board was NOT initialized"<<endl;
}
}

void Board2048::getInput(){
	char ch;
	bool go(false);
	while(!go && (ch = getch())){
		switch(ch){
		case 'w':
			b_way = 'U';
			go = true;
			break;
		case 's':
			b_way = 'D';
			go = true;
			break;
		case 'a':
			b_way = 'L';
			go = true;
			break;
		case 'd':
			b_way = 'R';
			go = true;
			break;
		case 'q':
			go = true;
			break;
		default:
			break;
		}
	}
	if(ch == 'q')
		showBoard('Q');	
}

void Board2048::moveBoard(){
	for(int i=0;i<b_row;i++)
		for(int j=0;j<b_col;j++)
		/* k is the separating space between adjacent Key */
			for(int k=1;k<b_col-j;k++){
				if(combineKey(i,j,k))
					break;
			}	 
}

bool Board2048::combineKey(int i,int j,int k){
	int r1,r2,c1,c2;
try
{
	switch(b_way){
	case 'U':
		r1 = j;c1 = i;
		r2 = j+k;c2 = i;
		break;
	case 'D':
		r1 = b_col-j-1;c1 = i;
		r2 = b_col-j-k-1;c2 = i;
		break;
	case 'L':
		r1 = i;c1 = j;
		r2 = i;c2 = j+k;
		break;
	case 'R':
		r1 = i;c1 = b_col-j-1;
		r2 = i;c2 = b_col-j-k-1;
		break;
	default:
		throw exception();
		break;
	}
	assert(r1>=0 && r1<b_row);
	assert(r2>=0 && r2<b_row);
	assert(c1>=0 && c1<b_col);
	assert(c2>=0 && c2<b_col);
	
}
catch(exception e){
	cout<<"Invalid direction!!"<<endl;
}
	/**********************
	 only A==B and A!=0 ,this Position is OK
	      A!=B and A!=0 and B!=0 ,this Position is OK
	 while OK means this Position do not need to check again! 
	*/
	if(board[r1][c1] == board[r2][c2] && board[r1][c1]){
		board[r1][c1] = 2*board[r1][c1];
		board[r2][c2] = 0;
		assert(keyNum>=2);
		/* Reduce the KeyNum */
		keyNum--;
		/* Set the MaxKey */
		if(board[r1][c1]>maxKey)
			maxKey = board[r1][c1];
		return true;
	}else if(board[r1][c1] && board[r2][c2] && board[r1][c1] != board[r2][c2]){	
		return true;
	}
	else if(!board[r1][c1]){
		board[r1][c1] = board[r2][c2];
		board[r2][c2] = 0;
	}
	return false;	
} 

/* set Num pieces of Keys */
Point Board2048::randPoint(){
	int pos = rand()%(b_row*b_col - getKeyNum())+1;
	int k = 0;
	for(int i=0;i<b_row;i++){
		for(int j=0;j<b_col;j++){
			if(!board[i][j]){
				if(++k == pos)
					return Point(i,j);
			}
		}
	}
	return Point(-1,-1);
}

bool Board2048::randKey(int Num){
try
	{
	for(int i=0;i<Num;i++){
		int value = rand()%2;
		Point pt = randPoint();
		if(pt.x == -1 && pt.y == -1)
			throw exception();
		board[pt.x][pt.y] = (value+1)*2;
		/*  Increase the KeyNum */
		keyNum++;
	}
	}
catch(exception e){
	cout<<"No extra Position!!"<<endl;
}
}

void Board2048::checkOver(){
	if(getMaxKey()>=2048){
		showBoard('O');
	}	
}

/******************
  'O' : 2048 is achieved!
  'Q' : quit is inputed!
  'N' : nomally display!
******************/
void Board2048::showBoard(char info){
	string question = "";
	switch(info){
	case 'O':
		question = "Congratulations!!! You have got the 2048!!!continue?(y/n)";
		break;
	case 'Q':
		question = "You want to quit really?(y/n)";
		break;
	default:
		break;
	}
	/* clear screen */
	system("clear");
	/* show LOGO */
	cout<<LOGO<<" \t\t";		
	cout<<"Board Row:"<<b_row<<" Col:"<<b_col<<" MaxKey:"<<maxKey<<endl;
	cout<<endl;
	cout<<question<<endl;
	/* show the board */
	cout<<"|";
	for(int i=0;i<b_col;i++)
		cout<<setw(5)<<"-----";
	cout<<"|"<<endl;
	for(int i=0;i<b_row;i++){
	
		cout<<"|";
		for(int j=0;j<b_col;j++)
			cout<<setw(5)<<board[i][j];
		cout<<"|"<<endl;
	}
	cout<<"|";
	for(int i=0;i<b_col;i++)
		cout<<setw(5)<<"-----";
	cout<<"|"<<endl;	
	/* input MSG */	
	if(info == 'Q' || info=='O'){
		char ch;
		while((ch = getch()) && (ch!='y' && ch!='n')){};
		if(ch == 'y')
			b_quit = 'q';
		else
			b_quit = 'n';
	}
}

/* 条件变量 当然要有个条件，如果nready已经不为0，就说明已经输入准备好，不必再等待！ */
int Board2048::getch(){
	pthread_mutex_lock(&shared.mutex);
	while(shared.nready==0)
	{
		pthread_cond_wait(&shared.cond, &shared.mutex);
	}
	shared.nready--;
	pthread_mutex_unlock(&shared.mutex);
	return shared.Way;
}

