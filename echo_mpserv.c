#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;									// sigaction함수를 사용하기 위한 구조체 선언
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if(argc != 2){
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_childproc;						// sa_handler -> 시그널 발생 시 호출할 함수 포인터 값을 저장
	sigemptyset(&act.sa_mask);								// 사용하지 않을 sigaction 구조체의 멤버 sa_mask의 모든 비트를 0으로 초기화
	act.sa_flags = 0;										// 사용하지 않을 멤버이므로 0으로 초기화
	// sigaction(시그널의 정보, 시그널 핸들러의 정보, 이전에 등록된 시그널 핸들러의 함수 포인터를 얻음(필요없으면 0))
	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while(1)
	{
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);	// 클라이언트 소켓 연결
		if(clnt_sock == -1)														// 연결할 클라이언트 소켓이 없을 경우
			continue;
		else
			puts("new client connected...");
		pid = fork();															// 동작을 실행할 자식 프로세스 생성
		if(pid == 0)															// 자식 프로세스인 경우
		{
			close(serv_sock);													// 이미 부모 서버에서 실행한 serv_sock을 닫는다
			while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);

			close(clnt_sock);													// 클라이언트에서 요청한 동작이 끝난 후 clnt_sock를 닫는다.
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id : %d \n", pid);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
