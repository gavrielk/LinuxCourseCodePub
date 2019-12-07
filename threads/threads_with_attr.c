// Must define for usage of non POSIX standard function(for *_np and sched_* APIs)
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>

#define assert_if(errnum) if (errnum !=0){printf("Error: %m\n"); exit(EXIT_FAILURE);}

#define POLICY_STR(policy)  (policy == SCHED_FIFO)  ? "SCHED_FIFO" :	\
		                  (policy == SCHED_RR)    ? "SCHED_RR" :	\
                   		  (policy == SCHED_OTHER) ? "SCHED_OTHER" :	\
		                  "???"
void* thread(void* arg1);

// Example of creating a thread and setting priority and sechduling policy
int main(int argc,  char* argv[])
{
	int res = -1, policy = -1;
	pthread_t pthread = -1;
	pthread_attr_t attr;
	cpu_set_t set;
	struct sched_param max_prio = {sched_get_priority_max(SCHED_FIFO)}; 
	struct sched_param min_prio = {sched_get_priority_min(SCHED_RR)};

	// Set Round-Robin policy and lowest priority to main thread	
	pthread_setschedparam(pthread_self(), SCHED_RR, &min_prio);
	
	res = pthread_attr_init(&attr);
	assert_if(res);
	
	// Force both threads to share the same CPU, otherwise priority is not necesserely inforced
	CPU_ZERO(&set);
	CPU_SET(0, &set);

	// Set main thread affinity to CPU 1
	res = sched_setaffinity(getpid(), sizeof(set), &set);
	assert_if(res);

	// Set child's thread affinity to CPU 1 using the attr
	 res = pthread_attr_setaffinity_np(&attr, sizeof(set), &set);
	assert_if(res);

	res = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	assert_if(res);

	res = pthread_attr_setschedparam(&attr, &max_prio);
	assert_if(res);

	res = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	assert_if(res);

	// Execute the child thread with the attributes
	res = pthread_create(&pthread, &attr, thread, NULL);
	assert_if(res);

	// Print Main thread policy and priority
	pthread_getschedparam(pthread_self(), &policy,  &min_prio);
	printf("Main priority %d, policy %s, core %d\n", min_prio.sched_priority, POLICY_STR(policy), sched_getcpu());

	res = pthread_join(pthread, NULL);
	assert_if(res);

	res = pthread_attr_destroy(&attr);
	assert_if(res);

	return 0;
}

void* thread(void* arg1)
{
	int res = -1, policy = -1;
	struct sched_param sp = {0};

	// Print Child thread policy and priority
	res = pthread_getschedparam(pthread_self(), &policy,  &sp);
	assert_if(res);

	printf("Child priority %d, policy %s, core %d\n", sp.sched_priority, POLICY_STR(policy), sched_getcpu());
	
	return NULL;
}
