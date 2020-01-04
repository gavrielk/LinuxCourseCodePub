#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>

#define MQ_MAX_SIZE         10
#define MQ_MAX_MSG_SIZE     4
#define MQ_NAME             "/my_mq"

void *Writer();
void *Reader();

int main()
{    
    pthread_t wtid, rtid;
    mqd_t mq_id;
	struct mq_attr attr = {0};

    /* initialize the queue attributes */
    attr.mq_maxmsg = MQ_MAX_SIZE;
    attr.mq_msgsize = MQ_MAX_MSG_SIZE;
    
    /* create the message queue with RW permission for current user and group */
	mq_id = mq_open(MQ_NAME, O_CREAT, S_IRWXU | S_IRWXG, &attr);
    
	/* Execute the threads and wait */
	pthread_create(&wtid,NULL,Writer,(void*)&mq_id);
    pthread_create(&rtid,NULL,Reader,(void*)&mq_id);

    pthread_join(wtid,NULL);
    pthread_join(rtid,NULL);
    
    return 0;
}

void *Writer(void *data)
{
    int index = 0;
    mqd_t mq_id = mq_open(MQ_NAME, O_WRONLY); // Open mq for writing

    for(;;)
    {
		/* Send index to the queue */
		index++;
        mq_send(mq_id, (char*)&index, MQ_MAX_MSG_SIZE, 0);
        printf("Wrote : %d \n",index);
    }    
}

void *Reader(void* data)
{
    int index = 0;
	mqd_t mq_id = mq_open(MQ_NAME, O_RDONLY); // Open mq for reading

    for(;;)
    {
		/* Read msg from queue into index and print it */
        mq_receive(mq_id, (char*)&index, MQ_MAX_MSG_SIZE, NULL);
        printf("Read : %d\n", index);
    }
}
