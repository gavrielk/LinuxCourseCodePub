/*
kill -SIGTERM <pid>

To see a list of all available signals on your system, you can use the kill -l command. 
This will print a list of signal names and their corresponding numbers

SIGINT (2) - Interrupt signal. This signal is typically sent to a process when the user presses Ctrl+C. It asks the process to terminate gracefully.

SIGTERM (15) - Termination signal. This signal asks the process to terminate gracefully, allowing it to clean up any resources it is using before exiting.

SIGKILL (9) - Kill signal. This signal forcefully terminates the process, without giving it a chance to clean up any resources.

SIGSTOP (19) - Stop signal. This signal stops the process, temporarily suspending its execution.

SIGCONT (18) - Continue signal. This signal resumes the execution of a process that has been stopped using SIGSTOP.

SIGHUP (1) - Hangup signal. This signal is typically sent to a process when its controlling terminal is closed or disconnected.

SIGUSR1 (10) and SIGUSR2 (12) - User-defined signals. These signals can be used by a process for any purpose.
*/

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <chrono>
#include <thread>

void signaHandler(int signal_number) 
{
    switch(signal_number) {
        case SIGINT:
			std::cout << "Received SIGINT signal" << std::endl;
            break;
        case SIGTERM:
			std::cout << "Received SIGTERM signal" << std::endl;
            break;
		case SIGSEGV:
			std::cout << "Received SIGSEGV signal" << std::endl;
			// Handle segmentation fault gracefully
			std::cout << "Segmentation fault occurred. Exiting gracefully" << std::endl;
			exit(1);
            break;
        case SIGUSR1:
			std::cout << "Received SIGUSR1 signal" << std::endl;
            break;
			
        default:
			std::cout << "Received unknown signal" << std::endl;

    }
}

int main(void)
{
	std::size_t i = 0;
	pid_t pid = getpid();

	if (signal(SIGINT, signaHandler) == SIG_ERR) {
		std::cerr << "Failed to set signal handler" << std::endl;
		return 1;
    }

	if (signal(SIGSEGV, signaHandler) == SIG_ERR) {
		std::cerr << "Failed to set signal handler" << std::endl;
		return 1;
    }

	if (signal(SIGTERM, signaHandler) == SIG_ERR) {
		std::cerr << "Failed to set signal handler" << std::endl;
		return 1;
    }

	if (signal(SIGUSR1, signaHandler) == SIG_ERR) {
		std::cerr << "Failed to set signal handler" << std::endl;
		return 1;
    }

	//Can we set a signal handler for SIGKILL?
//	 if (signal(SIGKILL, signaHandler) == SIG_ERR) {
//	 	std::cerr << "Failed to set signal handler" << std::endl;
//	 	return 1;
//     }

	std::cout << "Process with PID: " <<  pid << " is alive!!!!"  << std::endl;

    sigset_t mask;
    if (sigemptyset(&mask) == -1) { //initializes the signal set to exclude all signals.
        std::cerr << "Failed to initialize signal set" << std::endl;
        return 1;
    }
    if (sigaddset(&mask, SIGINT) == -1) { //adds a specified signal to the signal set.
        std::cerr << "Failed to add SIGINT to signal set" << std::endl;
        return 1;
    }
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) { //used to block or unblock the signal.
        std::cerr << "Failed to block SIGINT signal" << std::endl;
        return 1;
    }
    std::cout << "SIGINT signal blocked" << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(10));

	if (sigaddset(&mask, SIGINT) == -1 || sigaddset(&mask, SIGTERM) == -1  || sigaddset(&mask, SIGSEGV) == -1 || sigaddset(&mask, SIGUSR1) == -1 )  { //adds a specified signal to the signal set.
	std::cerr << "Failed to add Multiple signals to  set" << std::endl;
	return 1;
    }

    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        std::cerr << "Failed to unblock Multiple signals" << std::endl;
        return 1;
    }
	std::cout << "Multiple signals unblocked" << std::endl;

	while (1)
	{
		sleep(1);
		std::cout << "Process with PID: " <<  pid << " is still alive!!!!"  << std::endl;


//		 //Demonstrate the segmentation fault
//		 if(i == 3)
//		 {
//		 //Intentionally cause a segmentation fault
//		 	*(int*)0 = 0;
//		 }

		i++;

	}

	return 0;
}