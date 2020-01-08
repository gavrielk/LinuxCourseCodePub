#define MQ_MAX_SIZE         10
#define MQ_MAX_MSG_SIZE     100 		//Some big value(in bytes)
#define MQ_NAME             "/my_mq"

typedef enum{
	FLOAT,
	UINT
} NUMBER_TYPE_E;

/* Data that will be passed from the Writer to the reader
should hold the actual application data */ 
typedef struct msg{
	NUMBER_TYPE_E type;
	char data[]; // Dynamic/flexible array - place holder for unknown size data
}MSG_T;

typedef struct float_msg_data{
	float fvalue;
} FLOAT_MSG_DATA_T;

typedef struct uint_msg_data{
	unsigned int uvalue;
} UINT_MSG_DATA_T;


