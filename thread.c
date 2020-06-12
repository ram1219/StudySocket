#include <stdio.h>
#include <pthread.h>
#include <unistd.h>																	// sleep()을 위한 헤더
void* thread_main(void *arg);

// 함수 컴파일 시 : gcc 파일명 -o 실행파일명 -lsthread
int main(int argc, char *argv[])
{
	pthread_t t_id;																	// thread ID 생성
	int thread_param = 5;

	// pthread_create(thread ID의 주소값, thread에 부여할 특성(NULL시 기본), thread의 실행동작을 하는 함수의 주소,
	//				  thread 실행 함수가 호출될 때 전달되는 인자의 정보를 담고있는 변수의 주소) --> 성공 시 0, 실패 시 0이외의 값
	if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
	{
		puts("pthread_create() error");
		return -1;
	};
	sleep(10);
	puts("end of main");
	return 0;
}

// void *arg는 thread_param의값을 가져온다
void* thread_main(void *arg)
{
	int i;
	int cnt = *((int*)arg);									// int로 형 변환한 arg의 주소를 cnt에 전달
	for(i = 0; i < cnt; i++)
	{
		sleep(1);
		puts("running thread");
	}
	return NULL;
}
