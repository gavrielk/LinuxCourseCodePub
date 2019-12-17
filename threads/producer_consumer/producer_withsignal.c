# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define MAX_SIZE 10000

void *producer();
void *consumer();

int g_counter = 0;

// g_lock synchronizes access to g_counter
pthread_mutex_t g_lock;			// = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cv_not_empty;		// = PTHREAD_COND_INITIALIZER;

int main()
{    
    	pthread_t ptid,ctid;
   
		pthread_mutex_init(&g_lock, NULL);
		pthread_cond_init(&g_cv_not_empty, NULL);
		pthread_create(&ptid, NULL, producer, NULL);
    	pthread_create(&ctid, NULL, consumer, NULL);
    
    	pthread_join(ptid, NULL);
    	pthread_join(ctid, NULL);
    
    	return 0;
}

void *producer()
{    
	for(;;)
	{
		// Lock before reading and writing to g_counter
		pthread_mutex_lock(&g_lock);

        if(g_counter < MAX_SIZE)
	    {
			printf("Produce : %d \n",++g_counter);
	    }                   

		// Release mutex and broadcast the consumers
		pthread_mutex_unlock(&g_lock);
		pthread_cond_signal(&g_cv_not_empty);

		sleep(0.1);
	}    
}

void *consumer(void* arg)
{
    for(;;)
    {
		// Lock g_lock before reading g_count
		pthread_mutex_lock(&g_lock);
		if(g_counter == 0)
		{
			// Release g_lock and wait for not empty signal
			pthread_cond_wait(&g_cv_not_empty, &g_lock);
		}
		// At this point, g_lock is locked by pthread_cond_wait

		printf(".\n");
		// Read one
		if(g_counter != 0)
		{
			printf("Consume : %d\n", --g_counter);
		}

		// Release g_lock and repeat the loop
		pthread_mutex_unlock(&g_lock);
	}    
}
