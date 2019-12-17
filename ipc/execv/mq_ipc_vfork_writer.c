#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include "mq_ipc_vfork_include.h"

void main() //Writer
{
    MSG_DATA_S msg_data = {0};
    struct mq_attr mqAttr = {0};
	
	mqd_t mq = mq_open(MQ_NAME, O_WRONLY);

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
