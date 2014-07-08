#include<sys/socket.h>
#include<termios.h>
#include<string.h>
#define MAXLINE 128
#define SERV_PORT 9800
int main(int argc, char* argv[])
{
		int clientfd, connfd;
		struct sockaddr_in servaddr;
		int servlen;
		char sendbuf[MAXLINE];
		char ch;
		if((clientfd = socket(AF_INET, SOCK_STREAM, 0))<0){
					perror("socket failed!");
					exit(1);
		}
		bzero(&servaddr, sizeof(struct sockaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERV_PORT);
		if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr)<=0){
					perror("server address is wrong!");
					exit(1);
		}
		if((connfd = connect(clientfd, (struct sockaddr*)&servaddr, &servlen))<0){
					perror("connect failed!");
					exit(1);
		}
		while(1){
					 termios org_opts, new_opts;
			     tcgetattr(STDIN_FILENO, &org_opts);                                 //>����ԭ����
			     memcpy(&new_opts, &org_opts, sizeof(new_opts));
			     new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
			     tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
			     int c = getchar();
			     tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
					sendbuf[0] = c;
					sendbuf[1] = '/n';
					write(connfd, sendbuf, 2);
		}
		return 0;
}