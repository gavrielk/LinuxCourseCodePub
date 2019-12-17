#define MQ_MAX_SIZE         10
#define MQ_MAX_MSG_SIZE     sizeof(MSG_DATA_S)
#define MQ_NAME             "/my_mq"

/* Data that will be passed from the Writer to the reader
should hold the actual application data */ 
typedef struct msg_data{
	unsigned int counter;
}MSG_DATA_S;


