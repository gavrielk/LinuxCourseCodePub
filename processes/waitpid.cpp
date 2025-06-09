// $ kill -STOP pid
// stopped by signal 19
// $ kill -CONT pid
// continued
// $ kill -TERM pid
// killed by signal 15


//To spot zombi process 
//ps -elf | grep Z
//ps aux | grep defunct
//ps -la

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void signaHandler(int signal_number) 
{
    switch(signal_number) {
        case SIGTERM:
            printf("Received SIGTERM signal\n");
            fflush(stdout);
            break;
        case SIGUSR1:
            printf("Received SIGUSR1 signal\n");
            fflush(stdout);
            break;
        default:
            printf("Received unknown signal\n");
            fflush(stdout);
    }
}


int main(int argc, char *argv[])
{
    pid_t cpid, w;
    int status;

    cpid = fork();
    if (cpid == -1) 
    { 
        perror("fork"); 
        exit(EXIT_FAILURE); 
    }


    if (cpid == 0) 
    {   
        /* Code executed by child */
        printf("Child PID is %ld\n", (long) getpid());
        fflush(stdout);

        signal(SIGTERM, signaHandler);

        printf("Child PID is %ld, waiting on pause\n", (long) getpid());
        fflush(stdout);
        pause();                    /* Wait for signals */
        printf("Child PID is %ld, continue to execute program!!!\n", (long) getpid());
        fflush(stdout);


        //TO illustrate a zombie process
//         printf("Child PID is %ld,exiting!!! \n", (long) getpid());
//        fflush(stdout);

        _exit(EXIT_SUCCESS);
    }
    else 
    {

        printf("Parent PID is %ld\n", (long) getpid());
        fflush(stdout);

        /* Code executed by parent */

//         while(true)
//         {
//             printf("parent PID is alive %ld\n", (long) getpid());
//             sleep(3);
//         }


        do
        {

            w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) 
            { 
                perror("waitpid"); 
                exit(EXIT_FAILURE); 
            }

            if (WIFEXITED(status)) {
                printf("exited, status=%d\n", WEXITSTATUS(status));
                fflush(stdout);
            } else if (WIFSIGNALED(status)) {
                printf("killed by signal %d\n", WTERMSIG(status));
                fflush(stdout);
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
                fflush(stdout);
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
                fflush(stdout);
            }
        } 
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
        exit(EXIT_SUCCESS);
    }
}