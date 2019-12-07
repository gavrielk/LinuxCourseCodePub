# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>

# define MAX_SIZE 100

void *producer();
void *consumer();

int g_counter = 0;

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
        if(g_counter <= MAX_SIZE)
        {                        
		printf("Produce : %d \n",g_counter++);
        }                             
    }    
    
}

void *consumer()
{
    for(;;)
    {
        if(g_counter != -1)
        {            
		printf("Consume : %d\n",g_counter--);
        }                
    }    
}
