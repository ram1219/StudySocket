#include <stdio.h>
#include <unistd.h>

int gval = 10;													// 전역변수
int main(int argc, char *argv[])
{
	pid_t pid;													// pid_t fork()
																// --> 부모프로세스 : fork 함수의 반환값은 자식 프로세스의 ID,
																// --> 자식프로세스 : 반환값이 0
	int lval = 20;
	gval++, lval += 5;

	pid = fork();												// 자식 프로세스 생성
	printf("pid : %d \n", pid);									// 처음 -> 자식 프로세스의 ID 반환
																// 두번 째 -> 0 반환

	if(pid == 0)												// 자식 프로세스 일 경우
		gval += 2, lval += 2;
	else														// 부모 프로세스 일 경우
		gval -= 2, lval -= 2;

	if(pid == 0)
		printf("Child Proc : [%d, %d] \n", gval, lval);
	else
		printf("Parent Proc : [%d, %d] \n", gval, lval);
	return 0;
}
