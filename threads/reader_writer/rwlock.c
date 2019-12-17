# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define NUM_OF_READERS 3

void *Writer();
void *Reader();

int g_counter = 0;

// g_rwlock synchronizes access to g_counter
pthread_rwlock_t g_rwlock;

int main()
{    
    pthread_t ptid,ctid;
	pthread_rwlock_init(&g_rwlock, NULL);

    pthread_create(&ptid, NULL, Writer, NULL);
	for(int i = 0; i < NUM_OF_READERS; i++)
	{
    	pthread_create(&ctid, NULL, Reader, (int)i);
    }

    pthread_join(ptid, NULL);
    pthread_join(ctid, NULL);
    
    return 0;
}

void *Writer()
{    
	for(;;)
	{
		// Lock before reading and writing to g_counter
		pthread_rwlock_wrlock(&g_rwlock);
		
		for(int i = 0; i < 100; i++)
		{
			g_counter++;
			printf("Writer: %d \n",g_counter);
		}

		// Release write lock
		pthread_rwlock_unlock(&g_rwlock);
	}    
}

void *Reader(void* id)
{
	int i = 0;

   	for(;;)
   	{
		// Lock g_rwlock before reading g_count
		pthread_rwlock_rdlock(&g_rwlock);

		// Read delta
		while(i < g_counter)
		{
			i++;
			printf("Reader #%d : %d\n", (int)id, i);
		}

		// Release g_rwlock and repeat the loop
		pthread_rwlock_unlock(&g_rwlock);
	}    
}
