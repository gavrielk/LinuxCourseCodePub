#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

pthread_mutex_t g_lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_lock2 = PTHREAD_MUTEX_INITIALIZER;

void* dead(void* arg)
{
	pthread_mutex_lock(&g_lock1);
	sleep(2);
	pthread_mutex_lock(&g_lock2);
	return NULL;
}

void* lock(void* arg)
{
	pthread_mutex_lock(&g_lock2);
	sleep(2);
	pthread_mutex_lock(&g_lock1);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t dead_tid,lock_tid;
   
    	pthread_create(&dead_tid, NULL, dead, NULL);
    	pthread_create(&lock_tid, NULL, lock, NULL);
    
    	pthread_join(dead_tid, NULL);
    	pthread_join(lock_tid, NULL);
    
    	return 0;
}


