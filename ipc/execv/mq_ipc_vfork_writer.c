#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include "mq_ipc_vfork_include.h"

void main() //Writer
{
	float counter = 0;
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
			counter += 0.5;
			MSG_T* msg;

			// Precise allocation should be done according to the type
			// It is also possible to allocate a big buffer(see reader)
			if (counter == (unsigned int)counter)	// if integer use UINT_MSG_DATA_T
			{
				msg = malloc(sizeof(MSG_T) + sizeof(UINT_MSG_DATA_T));
				msg->type = UINT;
				((UINT_MSG_DATA_T*)msg->data)->uvalue = (unsigned int)counter;
			}
			else 									// if float use FLOAT_MSG_DATA_T
			{
				msg = malloc(sizeof(MSG_T) + sizeof(FLOAT_MSG_DATA_T));
				msg->type = FLOAT;
				((FLOAT_MSG_DATA_T*)msg->data)->fvalue = counter;
			}

            mq_send(mq, (char*)msg, MQ_MAX_MSG_SIZE, 0);
            printf("Writer: %.1f\n", counter);

			free(msg);
        }
    }    
}
