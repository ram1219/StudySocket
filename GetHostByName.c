#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int i;
	struct hostent *host;

	if(argc != 2){
		printf("Usage : %s <addr> \n", argv[0]);
		exit(1);
	}

	// gethostbyname(hostname) --> 성공 시 hostent 구조체 변수의 주소 값, 실패 시 NULL 반환
	host = gethostbyname(argv[1]);
	if(!host)
		error_handling("gethost... error");

	printf("Official name : %s \n", host->h_name);						// h_name --> hostent구조체의 멤버, 공식 도메인 이름

	for(i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i + 1, host->h_aliases[i]);			// h_aliases --> 도메인 별칭

	// h_addrtype --> 주소의 주소 체계 (IPv4뿐만 아니라  IPv6까지 지원한다)
	printf("Address type : %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

	// h_addr_list --> 도메인 이름에 대한 IP주소를 정수의 형태로 반환, 모든 IP의 주소정보를 얻을 수 있다.
	for(i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
