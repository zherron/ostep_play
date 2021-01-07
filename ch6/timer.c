/*
 * Measurement hw for ch. 6. Masure time costs of syscall and context switch
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  struct timeval *t_before = (struct timeval *)malloc(sizeof(struct timeval));
  struct timeval *t_after = (struct timeval *)malloc(sizeof(struct timeval));
  char buf[1];

  // get time before
  if (gettimeofday(t_before, NULL) != 0) {
    fprintf(stderr, "had a bad time getting time: %s\n", strerror(errno));
    exit(errno);
  }

  // perform zero byte read
  read(STDIN_FILENO, buf, 0);

  // get time after
  if (gettimeofday(t_after, NULL) != 0) {
    fprintf(stderr, "had a bad time getting time: %s\n", strerror(errno));
    exit(errno);
  }

  // get diff and print
  int diff = t_after->tv_usec - t_before->tv_usec;
  printf("time for zero byte read: %d us\n", diff);

  return 0;
}
