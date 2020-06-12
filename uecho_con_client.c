#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char *message);

// UDP 연결은 기본적으로 unconnected 소켓이다. --> 목적지 정보가 등록되어 있지 않음.(client에서 connect()함수를 사용하지 않음)
// 같은 호스트와 오랜 시간 데이터를 송수신할 경우 connected 소켓으로 만드는 것이 효율적이다.
int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	// socklen_t adr_sz;									    // 불필요해진 변수

	struct sockaddr_in serv_adr;  // , from_adr;				// 불필요해진 변수
	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	while(1)
	{
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		write(sock, message, strlen(message));

		/*adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);*/
		str_len = read(sock, message, sizeof(message) - 1);

		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

