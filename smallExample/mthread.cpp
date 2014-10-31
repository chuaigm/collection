#include <cstdio>
#include <unistd.h>
#include <pthread.h>

void *th_func1(void *data)
{
	int flag = 1;
	while(flag)
	{
		sleep(1);
		printf("-This is thread 1 --- 1s\n");
		fflush(stdout);
	}
}

void *th_func2(void *data)
{
	int flag = 1;
	while(flag)
	{
		sleep(2);
		printf("--This is thread 2 --- 2s\n");
		fflush(stdout);
	}
}

void *th_func3(void *data)
{
	int flag = 1;
	while(flag)
	{
		sleep(5);
		printf("-----This is thread 5 --- 5s\n");
		fflush(stdout);
	}
}

int main(int argc, char * argv[])
{
	printf(" -- test mthread -- \n");
	pthread_t th_a, th_b, th_c;
	void *retval;

	pthread_create(&th_a, NULL, th_func1, 0);
	pthread_create(&th_b, NULL, th_func2, 0);
	pthread_create(&th_c, NULL, th_func3, 0);

	pthread_join(th_a, &retval);
	pthread_join(th_b, &retval);
	pthread_join(th_c, &retval);


}

