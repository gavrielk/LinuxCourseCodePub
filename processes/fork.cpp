#include <stdio.h>
#include <iostream>
#include <unistd.h>

int main()
{
    pid_t cid = -1;
    printf("Hello, World!\n");
    int x =5;

    cid = fork();//<--
    if (cid > 0) 
    {
        /*parent process*/
        std::cout << "This is parent section [Process id: " <<  getpid() << "] , [child's id: " << cid << " ]." << std::endl;
        x++;
        //std::cout << "Parent section x= " << x << std::endl;
	printf("Parent section x= %d\n", x);

        
    }
    else if (cid == 0) 
    {
        /*child process*/
        std::cout << "Fork created [Process id: " <<  getpid() << ", [parent process id: " << getppid() << "]." << std::endl;
	    printf("Child section x= %d\n", x);
        //std::cout << "Child section x= " << x << std::endl;
    }
    else 
    {
        /* fork creation failed */
        std::cout << "fork creation failed!!!" << x << std::endl;
        return 1;
    }

    sleep(10000);

    return 0;
}
