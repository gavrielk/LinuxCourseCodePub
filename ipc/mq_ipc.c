#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>

#define MQ_MAX_SIZE         10
#define MQ_MAX_MSG_SIZE     sizeof(MSG_DATA_S)
#define MQ_NAME             "/my_mq"
#define NUM_OF_READERS    2

/* Data that will be passed from the Writer to the reader
should hold the actual application data */ 
typedef struct msg_data{
	unsigned int counter;
}MSG_DATA_S;

void Writer(mqd_t mq);
void Reader(mqd_t mq);

int main()
{    
    mqd_t mq;
    pthread_t wpid, rpid[NUM_OF_READERS];
    struct mq_attr attr = {0};

    /* initialize the queue attributes */
    attr.mq_maxmsg = MQ_MAX_SIZE;
    attr.mq_msgsize = MQ_MAX_MSG_SIZE;
    
    /* create the message queue and open it for read & write
	   children will inherit mq with attr from parent*/
    mq_unlink(MQ_NAME); // delete first if already exists, this requires sudo privilege
    mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG, &attr);

	/* create writer process */
	wpid = fork();
	if (wpid == 0) //Writer 
	{
		Writer(mq);
	}

	/* Create reader process */
    for(int i = 0; i < NUM_OF_READERS; i++)
    {
		rpid[i] = fork();
		if(rpid[i] == 0) //Reader
		{
			Reader(mq);
		}
    } 
   
	// Do nothing, only parent process should get here
	pause();

    return 0;
}

void Writer(mqd_t mq)
{
    MSG_DATA_S msg_data = {0};
    struct mq_attr mqAttr = {0};

    for(;;)
    {
		/* Check if there is place in the Q, if yes increment send, if no print error and try again */
        mq_getattr(mq, &mqAttr);
        if (mqAttr.mq_curmsgs == MQ_MAX_SIZE)
        {
            printf("Queue(%d) reached max number of messages(%ld)\n", mq, mqAttr.mq_maxmsg);
        }
        else
        {
			msg_data.counter++;
            mq_send(mq, (char*)&msg_data, MQ_MAX_MSG_SIZE, 0);
            printf("Writer: %u\n",msg_data.counter);
        }
    }    
}

void Reader(mqd_t mq)
{
    MSG_DATA_S msg_data = {0};
    struct mq_attr mqAttr = {0};

    for(;;)
    {
		/* Receive msg */
        mq_receive(mq, (char*)&msg_data, MQ_MAX_MSG_SIZE, NULL);

		/* Get attr for getting number of msgs currently in the Q*/
        mq_getattr(mq, &mqAttr);

		/* Print process ID, received counter and number of msgs left in the Q */
        printf("Reader(#%u): %u(remaining %ld messags in queue)\n", getpid(), msg_data.counter, mqAttr.mq_curmsgs);
    }
}
