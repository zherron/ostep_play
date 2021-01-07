/*
 * Measurement hw for ch. 6. time how long a context switch takes.
 * to do this I'll emulate the lmbench utility, which sets up a pipe
 * between two processes and writes from one to the other
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int fd[2];
  pid_t childpid;

  pipe(fd);

  if ((childpid = fork()) == -1) {
    fprintf(stderr, "bad fork: %s\n", strerror(errno));
    exit(errno);
  }

  // if we're in the child
  if (childpid == 0) {
    char buf[1];
    read(fd[1], buf, 1);
    write(fd[0], buf, 1);
  } else {
    struct timeval *t_before = (struct timeval *)malloc(sizeof(struct timeval));
    struct timeval *t_after = (struct timeval *)malloc(sizeof(struct timeval));
    // in the parent
    char pbuf[1];

    // get time before
    if (gettimeofday(t_before, NULL) != 0) {
      fprintf(stderr, "had a bad time getting time: %s\n", strerror(errno));
      exit(errno);
    }

    // write then read from pipes
    write(fd[1], pbuf, 1);
    read(fd[0], pbuf, 1);

    // get time after
    if (gettimeofday(t_after, NULL) != 0) {
      fprintf(stderr, "had a bad time getting time: %s\n", strerror(errno));
      exit(errno);
    }

    // get diff and print
    int diff = t_after->tv_usec - t_before->tv_usec;
    printf("time for two context switches: %d us\n", diff);
  }

}
