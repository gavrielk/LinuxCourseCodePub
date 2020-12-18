/*
 * MTA randomization utility library providing random data
 * Few APIs to get random data that doesn't repeats itself(unlike regular rand())
 * Author: Gabi Karasin
 */

/*
 * Function:    MTA_get_rand_data
 * Description: Fill buffer with random data that generates a seed based on a monotonic clock(always incrementing) in nanosecond resolution 
 * Note:        For extra randomness don't clean the data buffer between each call
 * --------------------------------------------------------------------------------------------
 * [out]    data        - buffer for the random data
 * [in]     data_length - length in bytes of the data variable
 */
void MTA_get_rand_data(char *data, unsigned int data_length);

/*
 * Function:    MTA_get_rand_char
 * Description: Same as above but for a single bytes/character
 * --------------------------------------------------------------------------------------------
 * [return] char        - random 1 byte(value between 0 and 255)
 */
char MTA_get_rand_char();
