#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

void* thread(void* arg1);

int main(int argc, char* argv[])
{
	int res = 0;
	void* join_res;

	//Get number of threads to be created from user input
	unsigned int num_of_threads = atoi(argv[1]);

	//Allocate arrays whose size depends on user input
	pthread_t* pthread_id_arr = malloc(num_of_threads * sizeof(pthread_t));
	unsigned int* input_args = malloc(num_of_threads * (sizeof(unsigned int)));

	//Create threads
	for(unsigned int i = 0; i < num_of_threads; i++)
	{
		input_args[i] = i;
		res = pthread_create(&pthread_id_arr[i], NULL, thread, (void*)&input_args[i]);
		assert(res == 0);
	}

	//Read output data from created threads
	for(unsigned int i = 0; i < num_of_threads; i++)
	{
		res = pthread_join(pthread_id_arr[i], &join_res);
		assert(res == 0);

		printf("Thread #%lu returned with value %u\n", pthread_id_arr[i], *(unsigned int*)join_res);
	}

	//Free allocated resources 
	free(input_args);
	free(pthread_id_arr);

	return res;
}

//Print and multiply input by 2
//Note: Input buffer is used also as output(should be avoided in general)
void* thread(void* arg1)
{
	printf("Thread #%lu created, input arg = %u\n", pthread_self(), *(unsigned int*)arg1);
	*(unsigned int*)arg1 *= 2;
	return arg1;
}
