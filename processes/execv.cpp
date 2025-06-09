#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include "path_utils.h"

int main()
{
    utils::path::print_working_directory();
    std::filesystem::path my_working_dir = "/tmp/linux_course/dup2/";
    utils::path::create_path(my_working_dir);
    utils::path::change_working_directory(my_working_dir);

    printf("This PID is: %d.\n",getpid());
    char * fileName = "tricky.txt";
    int file_desc = open(fileName, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (file_desc == -1) 
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("I will be printed to the terminal as standard output.\n");
    fflush(stdout);

    if (dup2(file_desc, STDOUT_FILENO) == -1) 
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
          
    // All the printf statements will be written to the file "tricky.txt".
    printf("I will be printed in the file %s.\n",fileName);
    fflush(stdout);

    if (close(file_desc) == -1) 
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}