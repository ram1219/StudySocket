#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid = fork();

	if(pid == 0)
	{
		sleep(15);
		return 24;
	}
	else
	{
		// waitpid -> 좀비 프로세스의 생성을 막고, wait함수의 블로킹 문제를 해결한다
		// waitpid(종료를 확인할 자식 프로세스의 ID, 매개변수, WNOHANG일 경우 자식 프로세스가 존재하지 않으면 0을 반환)
		while(!waitpid(-1, &status, WNOHANG))
		{
			sleep(3);
			puts("sleep 3sec.");
		}

		// WIFEXITED -> 자식 프로세스가 정상 종료한 경우 true을 반환
		if(WIFEXITED(status))									// 정상 종료 하였는가?
			printf("Child send %d \n", WEXITSTATUS(status));	// WEXITSTATUS -> 자식 프로세스의 전달 값을 반환
	}
	return 0;
}
