#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	int snd_buf = 1024*3, rcv_buf = 1024*3;
	int state;
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	// setsockopt(옵션변경을 위한 소켓, (변경할)옵션의 프로토콜 레벨, 옵션의 이름, 옵션정보를 저장한 버퍼의 주소, 옵션정보의 크기(바이트))
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
	if(state)
		error_handling("setsockopt() error");

	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
	if(state)
		error_handling("setsockopt() error");

	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if(state)
		error_handling("getsockopt() error");

	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if(state)
		error_handling("getsockopt() error");

	printf("Input buffer size : %d \n", rcv_buf);
	printf("Output buffer size : %d \n", snd_buf);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
