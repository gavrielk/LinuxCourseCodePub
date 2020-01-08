#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include "mq_ipc_vfork_include.h"

void main() //Reader
{
    MSG_T* msg = malloc(MQ_MAX_MSG_SIZE); // Allocate big size in advance
    struct mq_attr mqAttr = {0};

	mqd_t mq = mq_open(MQ_NAME, O_RDONLY);

    for(;;)
    {
		/* Receive msg */
        mq_receive(mq, (char*)msg, MQ_MAX_MSG_SIZE, NULL);

		/* Get attr for getting number of msgs currently in the Q*/
        mq_getattr(mq, &mqAttr);

		// Cast to concrete type
		if (msg->type == UINT)
		{
			unsigned int ucounter = ((UINT_MSG_DATA_T*)msg->data)->uvalue;
			printf("Reader(#%u): %u(remaining %ld messags in queue)\n", getpid(), ucounter, mqAttr.mq_curmsgs);
		}
		else
		{
			float fcounter = ((FLOAT_MSG_DATA_T*)msg->data)->fvalue;
			printf("Reader(#%u): %.1f(remaining %ld messags in queue)\n", getpid(), fcounter, mqAttr.mq_curmsgs);
		}
    }
}
