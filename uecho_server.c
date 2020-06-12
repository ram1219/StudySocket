#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

// UDP의 서버와 클라이언트는 연결되어 있지 않다.
int main(int argc, char *argv[])
{
	int serv_sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t clnt_adr_sz;

	struct sockaddr_in serv_adr, clnt_adr;
	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock == -1)
		error_handling("UDP socket creation error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	// 서버와 클라이언트를 연결하지 않으므로, TCP에서 연결에 사용한 listen(), accept()함수가 필요하지 않다.

	while(1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		// recvfrom(UDP소켓,수신에 사용될  버퍼의 주소 값, 수신할 최대 바이트 수, 옵션에 사용되는 변수(없으면 0), sockaddr 구조체 변수의 주소 값,
		//			전달된 주소에 해당하는 구조체 변수의 크기정보를 담고있는 변수의 주소값)  --> 성공 시 수신한 바이트 수, 실패 시 -1 반환
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		// sendto(UDP소켓, 데이터를 저장한 버퍼의 주소 값, 전송할 데이터의 크기, 옵션변수(없으면 0), sockaddr구조체 변수의 주소 값,
		//		  전달된 주소 값의 구조체 변수 크기) --> 성공 시 전송된 바이트 수, 실패 시 -1 반환
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

