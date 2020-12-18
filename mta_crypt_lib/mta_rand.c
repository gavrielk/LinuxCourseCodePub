#include "mta_rand.h"
#include <time.h>
#include <stdlib.h>

void MTA_get_rand_data(char* data, unsigned int data_length)
{
    int seed = 0;
    struct timespec curr_time = {0};

    clock_gettime(CLOCK_MONOTONIC, &curr_time);
    seed = (int)curr_time.tv_nsec + *(int*)data;

    for (int i = 0; i < data_length; i++)
    {
        data[i] = rand_r(&seed) % 255;
    }
}

char MTA_get_rand_char()
{
    int seed = 0;
    struct timespec curr_time = {0};

    clock_gettime(CLOCK_MONOTONIC, &curr_time);
    seed = (int)curr_time.tv_nsec;
    
    return (rand_r(&seed) % 255);
}
