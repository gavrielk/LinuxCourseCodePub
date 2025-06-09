//pip example

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//globals
int	ExitStatus = 0;
char Command[] = "ls -l";	
char StdIn[4096] = {0};
char StdOut[4096] = {0};
char StdErr[4096] = {0};

int READ_END = 0;
int WRITE_END = 1;

int infd[2] = {0, 0};
int outfd[2] = {0, 0};
int errfd[2] = {0, 0};

void cleanup()
{
	close(infd[READ_END]);
	close(infd[WRITE_END]);

	close(outfd[READ_END]);
	close(outfd[WRITE_END]);

	close(errfd[READ_END]);
	close(errfd[WRITE_END]);
}

void execute()
{
	int rc = pipe(infd);
	if(rc < 0)
	{
		printf("runtime error: %s", strerror(errno));
		return;
	}

	rc = pipe(outfd);
	if(rc < 0)
	{
		close(infd[READ_END]);
		close(infd[WRITE_END]);
		printf("runtime error: %s", strerror(errno));
		return;
	}

	rc = pipe(errfd);
	if(rc < 0)
	{
		close(infd[READ_END]);
		close(infd[WRITE_END]);

		close(outfd[READ_END]);
		close(outfd[WRITE_END]);
		printf("runtime error: %s", strerror(errno));
		return;
	}

	int pid = fork();
	if(pid > 0) // PARENT
	{
		close(infd[READ_END]); 	    // Parent does not read from stdin
		close(outfd[WRITE_END]);	// Parent does not write to stdout
		close(errfd[WRITE_END]);	// Parent does not write to stderr

		// if(write(infd[WRITE_END], StdIn, sizeof(StdIn)*4096 ) < 0)
		// {
		// 	printf("runtime error: %s", strerror(errno));
		// }
		close(infd[WRITE_END]); // Done writing
	}
	else if(pid == 0) // CHILD
	{
		dup2(infd[READ_END], STDIN_FILENO);
		dup2(outfd[WRITE_END], STDOUT_FILENO);
		dup2(errfd[WRITE_END], STDERR_FILENO);

		close(infd[WRITE_END]); 	// Child does not write to stdin
		close(outfd[READ_END]); 	// Child does not read from stdout
		close(errfd[READ_END]);		// Child does not read from stderr

		execl("/bin/bash", "bash", "-c", Command, nullptr);
		exit(EXIT_SUCCESS);
	}

	// PARENT
	if(pid < 0)
	{
		cleanup();
		printf("runtimr error: %s", strerror(errno));
	} 

	int status = 0;
	waitpid(pid, &status, 0);

	int chunck = 256;
	char buffer[chunck] ;
	ssize_t readSize = chunck/8;

	ssize_t bytes = 0;

	int k = 0;
	do
	{
		bytes = read(outfd[READ_END], buffer, readSize);
		if(bytes == 0)
			break;

        for (int i = 0; i < strlen(buffer); i++)
            StdOut[k++] = buffer[i];

	}
	while(bytes > 0);

	StdOut[k++] = '\0'; // string ends with '\0'

	k = 0;
	do
	{
		bytes = read(errfd[READ_END], buffer, readSize);

		if(bytes == 0)
			break;

        for (int i = 0; i < strlen(buffer); i++)
            StdErr[k++] = buffer[i];
	}
	while(bytes > 0);

	StdErr[k++] = '\0'; // string ends with '\0'


	if(WIFEXITED(status))
	{
		ExitStatus = WEXITSTATUS(status);
	}

	cleanup();
}


int main()
{
    std::cout << "running ls -l command" << std::endl;
	execute();

	printf("StdOut: %s\n", StdOut);
	printf("StdErr: %s\n", StdErr);

   return 0;
}