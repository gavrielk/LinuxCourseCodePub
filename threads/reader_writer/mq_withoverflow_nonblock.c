#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>
#include <errno.h>

#define MQ_MAX_SIZE         10
#define MQ_MAX_MSG_SIZE     sizeof(MSG_DATA_S)
#define MQ_NAME             "/my_mq"
#define NUM_OF_READERS    2

/* Cookie that will be passed from the parent thread to the child
containing the msg Q id and the thread ID(for printing) */
typedef struct reader_cookie{
    short int id;
    char mq_name[sizeof(MQ_NAME)];
}READER_COOKIE_S;

/* Data that will be passed from the Writer to the reader
should hold the actual application data */ 
typedef struct msg_data{
	unsigned int counter;
}MSG_DATA_S;

void *Writer();
void *Reader();

int main()
{    
    mqd_t mq;
    pthread_t wtid, rtid[NUM_OF_READERS];
    struct mq_attr attr = {0};
    READER_COOKIE_S readers_cookie[NUM_OF_READERS];
	char queue_name[sizeof(MQ_NAME)] = {MQ_NAME};

    /* initialize the queue attributes */
    attr.mq_maxmsg = MQ_MAX_SIZE;
    attr.mq_msgsize = MQ_MAX_MSG_SIZE;
    
    /* create the message queue */
    mq_unlink(MQ_NAME);
    mq = mq_open(MQ_NAME, O_CREAT, S_IRWXU | S_IRWXG, &attr);

	/* create writer thread and pass it the queue name */
    pthread_create(&wtid, NULL, Writer, (void*)&queue_name);
    
	/* Create consumer threads */
    for(int i = 0; i < NUM_OF_READERS; i++)
    {
		/* Fill the cookie attributes(id and msg Q id) and to thread */
        readers_cookie[i] = (READER_COOKIE_S){i, MQ_NAME};
        pthread_create(&rtid[i], NULL, Reader, (void*)&readers_cookie[i]);
    } 

	/* Wait for threads to finish */
    pthread_join(wtid,NULL);

    for(int i = 0; i < NUM_OF_READERS; i++)
        pthread_join(rtid[i],NULL);
    
    return 0;
}

void *Writer(void *data)
{
    MSG_DATA_S msg_data = {0};
    struct mq_attr mqAttr = {0};
	int ret = 0;

	/* Extract queue name from the cookie(note: it differs from the reader cookie) */
    mqd_t mq = mq_open((char*)data, O_WRONLY | O_NONBLOCK);

    for(;;)
    {
		msg_data.counter++;
        ret = mq_send(mq, (char*)&msg_data, MQ_MAX_MSG_SIZE, 0);
		if (errno == EAGAIN)
		{
        	mq_getattr(mq, &mqAttr);
           	printf("Queue(%d) reached max number of messages(%ld)\n", mq, mqAttr.mq_maxmsg);
			continue;
		}
        printf("%d Writer: %u\n",ret, msg_data.counter);
    }    
}

void *Reader(void* data)
{
    MSG_DATA_S msg_data = {0};
    struct mq_attr mqAttr = {0};

	/* Extract data from the cookie */
    unsigned int id = ((READER_COOKIE_S*)data)->id;
    mqd_t mq = mq_open((char*)((READER_COOKIE_S*)data)->mq_name, O_RDONLY);

    for(;;)
    {
		/* Receive msg */
        mq_receive(mq, (char*)&msg_data, MQ_MAX_MSG_SIZE, NULL);

		/* Get attr for getting number of msgs currently in the Q*/
        mq_getattr(mq, &mqAttr);

		/* Print thread ID, received counter and number of msgs left in the Q */
        printf("Reader(#%u): %u(remaining %ld messags in queue)\n", id, msg_data.counter, mqAttr.mq_curmsgs);
    }
}
