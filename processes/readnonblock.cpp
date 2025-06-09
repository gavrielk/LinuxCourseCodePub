#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int fd = open("my_pipe", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
      perror("open");
      return EXIT_FAILURE;
    }

    while (1) {
          char buffer[BUFSIZ];
          ssize_t len = read(fd, buffer, sizeof buffer - 1);
          if (len < 0) {
            perror("read");
            return EXIT_FAILURE;
          }
          buffer[len] = '\0';
          printf("Read: %s\n", buffer);
          if (len > 0){
            exit(0);
          }
        }
}