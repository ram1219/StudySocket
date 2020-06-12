#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100
void* thread_inc(void *arg);					// 전역변수의 값을 증가시키는 쓰레드
void* thread_des(void *arg);					// 전역변수의 값을 감소시키는 쓰레드

long long num = 0;								// 전역변수 num
pthread_mutex_t mutex;							// 전역변수에 쓰레드의 동시 접근을 방지하기 위한 mutex

// mutex는 전역변수를 사용 하기 전 lock을 하고 사용 후 unlock을 하므로써 동시 접근을 방지한다.
int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	pthread_mutex_init(&mutex, NULL);			// mutex 생성

	for(i = 0; i < NUM_THREAD; i++)
	{
		if(i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for(i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result : %11d \n", num);
	pthread_mutex_destroy(&mutex);				// mutex 제거
	return 0;
}

void* thread_inc(void *arg)
{
	int i;
	// 임계 영역 시작
	pthread_mutex_lock(&mutex);
	for(i = 0; i < 50000000; i++)
		num += 1;
	// 임계 영역 종료
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* thread_des(void *arg)
{
	int i;
	for(i = 0; i < 50000000; i++)
	{
		pthread_mutex_lock(&mutex);
		num -= 1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
