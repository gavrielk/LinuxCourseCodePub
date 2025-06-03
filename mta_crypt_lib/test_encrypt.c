#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <limits.h>
#include "getopt.h"
#include <mta_crypt.h>
#include <mta_rand.h>
#include "errno.h"
#include <sys/time.h>
#include <assert.h>

#define NUM_OF_ROUNDS       10
#define NUM_OF_ITERATIONS   10000000
#define PASS_LEN            24
#define KEY_LEN             PASS_LEN / 8

int main(int argc, char **argv)
{   
	struct timeval iter_start_time = {0}, iter_end_time = {0}, total_start_time = {0}, total_end_time = {0}; 
    long long int iter_elapsed = 0, total_elapsed = 0;
    char pass[200] = {0}, key[200] = {0}, enc_pass[200] = {0}, guessed_key[200] = {0}, dec_pass[200] = {0};
    int pass_len = PASS_LEN, key_len = KEY_LEN, enc_pass_len = 0, dec_pass_len = 0, iter_dec_counter = 0, total_dec_counter = 0;

    printf ("***** Start benchmark test *****\n");
    printf ("Running %d iterations in %d rounds\n", NUM_OF_ITERATIONS, NUM_OF_ROUNDS);

    MTA_CRYPT_RET_STATUS res = MTA_crypt_init();
    assert(res == MTA_CRYPT_RET_OK);

    gettimeofday(&total_start_time, 0);
    for (int j = 0; j < NUM_OF_ROUNDS; j++)
    {
        MTA_get_rand_data(pass, pass_len);
        MTA_get_rand_data(key, key_len);
        MTA_CRYPT_RET_STATUS res = MTA_encrypt(key, key_len, pass, pass_len, enc_pass, &enc_pass_len);
        assert(res == MTA_CRYPT_RET_OK);

        gettimeofday(&iter_start_time, 0);
        for (int i = 0; i < NUM_OF_ITERATIONS; i++)
        {
            MTA_get_rand_data(guessed_key, key_len);
            res = MTA_decrypt(guessed_key, key_len, enc_pass, enc_pass_len, dec_pass, &dec_pass_len);
            assert(res == MTA_CRYPT_RET_OK);
            
            if(memcmp(dec_pass, pass, pass_len) == 0){
                iter_dec_counter++;
                // printf("Password %s decrypted successfully\n", dec_pass);
            }
            bzero(guessed_key, 200);
            bzero(dec_pass, 200);
        }
        gettimeofday(&iter_end_time, 0);


        iter_elapsed = ((iter_end_time.tv_sec - iter_start_time.tv_sec) * 1000) + ((iter_end_time.tv_usec - iter_start_time.tv_usec) / 1000);
        printf("Round #%d: %d iterations took %lld miliseconds, decrepted %d passwords\n", j, NUM_OF_ITERATIONS, iter_elapsed, iter_dec_counter);
        total_dec_counter += iter_dec_counter;
        iter_dec_counter = 0;
    }

	gettimeofday(&total_end_time, 0);
    total_elapsed = ((total_end_time.tv_sec - total_start_time.tv_sec) * 1000) + ((total_end_time.tv_usec - total_start_time.tv_usec) / 1000);
    printf("***** End benchmark test *****\n");
    printf("Total time is %lld miliseconds\n", total_elapsed);
    printf("Total num of decrypted password is %d\n", total_dec_counter);

    return 0;
}