#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in addr1, addr2;
	char *str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr = htonl(0x1020304);
	addr2.sin_addr.s_addr = htonl(0x1010101);

	// char *inet_ntoa(sockaddr_in 구조체가 가진 IP주소 값) --> 빅 엔디안으로 정렬된 정수형 IP주소를사람이 보기 쉬운 문자열의 형태로 변환
	str_ptr = inet_ntoa(addr1.sin_addr);
	strcpy(str_arr, str_ptr);								// inet_ntoa() 함수는 함수 내부적으로 메모리 공간을 할당해서 변환된 정보를 저장.
															// --> 함수 재 호출 후 반환된 문자열 정보를 다른 메모리 공간에 복사
	printf("Dotted-Decimal notation1: %s \n", str_ptr);

	inet_ntoa(addr2.sin_addr);								// str_ptr에 자동으로inet_ntoa()함수로 반환된 문자열 정보가 저장이 된다.
															// 함수 내부적 공간에 할당되었기 때문에 함수가 가지고 있는 메모리 공간이다.
	printf("Dotted-Decimal notation2 : %s \n", str_ptr);
	printf("Dotted-Decimal notation3 : %s \n", str_arr);	// 앞서 str_arr에 복사해 둔 값을 호출한다.
	return 0;
} 
