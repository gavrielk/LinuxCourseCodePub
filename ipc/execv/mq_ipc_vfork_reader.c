#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include "mq_ipc_vfork_include.h"

void main() //Reader
{
    MSG_DATA_S msg_data = {0};
    struct mq_attr mqAttr = {0};

	mqd_t mq = mq_open(MQ_NAME, O_RDONLY);

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
