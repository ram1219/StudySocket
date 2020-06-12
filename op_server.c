#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char *message);						// 에러 메시지 출력함수
int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];								// 클라이언트에서 받은 입력값을 저장할 배열
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;
	struct sockaddr_in serv_addr;						// (서버)주소체계, 포트번호, IP주소를 멤버로 가지는 구조체
	struct sockaddr_in clnt_addr;						// (클라이언트)
	socklen_t clnt_addr_size;							// 클라이언트 소켓 주소 길이

	if(argc != 2){										// 입력받은 포트넘버가 없을 경우, 예외처리
		printf("Usage :%s <port>\n", argv[0]);			// ./po_server 9000 으로 실행할 경우 -> argv[0] = po_server, argv[1] = 9000, argc = 2
		exit(1);										// 프로그램 종료 (1 --> 비정상적인 종료, 에러로 인한 종료)
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);		// TCP로 소켓 생성
	if(serv_sock == -1)									// 소켓이 생성되지 않은 경우, 예외처리
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));			// 구조체의 모든 멤버 0으로 초기화
	serv_addr.sin_family = AF_INET;						// 주소체계 지정  --> IPv4 (AF_INET과 PF_INET은 헤더에 전처리되어있어 같은 의미)
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		// 문자열 기반의 IP주소 초기화
														// (INADDR_ANY -> 현재 사용하는 컴퓨터의 IP주소가 자동으로 할당)
	serv_addr.sin_port = htons(atoi(argv[1]));			// 문자열 기반의 port번호 초기화
														// htons (바이트 변환 함수) -->  h(호스트) to(에서) n(네트워크) s(short형)
														// short형 데이터를 호스트 바이트 순서에서 네트워크 바이트 순서로 변환

	// bind(주소를 할당할 소켓, 구조체 변수의 주소값, 구조체변수의 길이) --> 성공 시 0반환, 실패 시 -1 반환
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// listen(연결요청 대기상태로 둘 소켓, 대기 큐(Queue)의 크기정보) --> 성공 시 0, 실패 시 -1
	if(listen(serv_sock, 5) == -1)						// 클라이언트의 연결요청을 5개까지 대기가능
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);

	for(i = 0; i < 5; i++)
	{
		opnd_cnt = 0;

		// accept(서버 소켓, 연결요청 클라이언트의 주소정보를 담을 주소, 전달된 주소의 길이가 저장된 변수의 주소) --> 실패 시 -1
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		// 
		read(clnt_sock, &opnd_cnt, 1);

		recv_len = 0;
		while((opnd_cnt*OPSZ+1) > recv_len)
		{
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);	// calculate() 함수의 리턴값 저장
		// write(쓸(write) 소켓, 값을 저장한 변수의 주소, 변수의 길이)
		write(clnt_sock, (char*)&result, sizeof(result));
		close(clnt_sock);													// 클라이언트 소켓의 연결을 끊는다.
	}
	close(serv_sock);														// 소켓 연결해제
	return 0;
}

int calculate(int opnum, int opnds[], char op)
{
	int result = opnds[0], i;
	switch(op)
	{
	case '+':
		for(i = 1; i < opnum; i++) result += opnds[i];
		break;
	case '-':
		for(i = 1; i < opnum; i++) result -= opnds[i];
	    break;
	case '*':
		for(i = 1; i < opnum; i++) result *= opnds[i];
		break;
	case '/':
		for(i = 1; i < opnum; i++) result /= opnds[i];
		break;
	}
	return result;
}

void error_handling(char *message)
{
	fputs(message, stderr);								// stderr == 2
	fputc('\n', stderr);
	exit(1);
}
