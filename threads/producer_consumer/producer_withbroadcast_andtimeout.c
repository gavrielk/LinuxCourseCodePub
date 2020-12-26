# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <errno.h>

#define MAX_SIZE 10000
#define TIME_TO_WAIT 2

void *producer();
void *consumer();

int g_counter = 0;

// g_lock synchronizes access to g_counter
pthread_mutex_t g_lock;			// = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cv_not_empty;		// = PTHREAD_COND_INITIALIZER;

int main()
{    
    	pthread_t ptid,ctid;
	int consumer_thread_id[2] = {1, 2};
   
	pthread_mutex_init(&g_lock, NULL);
	pthread_cond_init(&g_cv_not_empty, NULL);
	pthread_create(&ptid, NULL, producer, NULL);
    	pthread_create(&ctid, NULL, consumer, &consumer_thread_id[0]);
    	pthread_create(&ctid, NULL, consumer, &consumer_thread_id[1]);
    
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
			g_counter += 100;
			printf("Produce : %d \n",g_counter);
	        }                   

		// Release mutex and broadcast the consumers
		pthread_mutex_unlock(&g_lock);
		pthread_cond_broadcast(&g_cv_not_empty);

		sleep(TIME_TO_WAIT + 1);
	}    
}

void *consumer(void* id)
{
		int res = 0;
		struct timespec timeout;
    	for(;;)
    	{
			// Lock g_lock before reading g_count
			pthread_mutex_lock(&g_lock);
			if(g_counter == 0)
			{
				// Release g_lock and wait for not empty signal or timeout to occure
				clock_gettime(CLOCK_REALTIME, &timeout);
				timeout.tv_sec += TIME_TO_WAIT;

				res = pthread_cond_timedwait(&g_cv_not_empty, &g_lock, &timeout);
				if (res == ETIMEDOUT)
				{
					printf("Have been waiting enough(%ds)! Exiting!!!\n", TIME_TO_WAIT);
					exit(-1);
				}

			}
		// At this point, g_lock is locked by pthread_cond_wait

		// Read one
			if(g_counter != 0)
			{
				printf("Consume #%d : %d\n", *(int*)id, g_counter--);
			}

			// Release g_lock and repeat the loop
			pthread_mutex_unlock(&g_lock);
	}    
}
