# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>

# define MAX_SIZE 10000

void *producer();
void *consumer();

int g_counter = 0;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

int main()
{    
    pthread_t ptid,ctid;
    
    pthread_create(&ptid,NULL,producer,NULL);
    pthread_create(&ctid,NULL,consumer,NULL);
    
    pthread_join(ptid,NULL);
    pthread_join(ctid,NULL);
    
    return 0;
}

void *producer()
{    
    for(;;)
    {
		// Lock before reading and writing to g_counter
		pthread_mutex_lock(&g_lock);

        if(g_counter <= MAX_SIZE)
        {                        
			printf("\nProduce : %d \n",++g_counter);
        }
		// Release mutex and broadcast the consumers
		pthread_mutex_unlock(&g_lock);
		usleep(100);
    }    
    
}

void *consumer()
{
    for(;;)
    {
		// Lock g_lock before reading g_count
		pthread_mutex_lock(&g_lock);
		printf(".");
		
        if(g_counter != 0)
        {            
			printf("\nConsume : %d\n",g_counter--);
        }

		// Release g_lock and repeat the loop
		pthread_mutex_unlock(&g_lock);
    }    
}
