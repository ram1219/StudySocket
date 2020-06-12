#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[]){
	int sock;
	int snd_buf, rcv_buf, state;
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	len = sizeof(snd_buf);
	// getsockopt(옵션을 확인할 소켓, 옵션의 프로토콜 레벨, 옵션의 이름, 확인결과 저장을 위한 버퍼의 주소, 버퍼크기를 담을 변수의 주소)
	// 소켓의 옵션을 확인할 때 호출하는 함수
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if(state)
		error_handling("getsockopt() error");

	printf("Input buffer size : %d \n", rcv_buf);
	printf("Output buffer size : %d \n", snd_buf);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
