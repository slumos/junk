#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Consume stdin and report chars/lines per second to stderr.
 *
 * e.g.  tail -f access.log | grep ^GET | lps
 */

int main(int argc, char** argv)
{
  unsigned long total_lines = 0;
  unsigned long chunk_lines = 0;
  unsigned long total_chars = 0;
  unsigned long chunk_chars = 0;
  struct timeval tv_start = {0};
  struct timeval tv_now = {0};
  struct timeval tv_delta = {0};
  double total_seconds = 0;
  double last_seconds = 0;

  ssize_t len, i;
  char buf[BUFSIZ];

  gettimeofday(&tv_start, NULL);
  while (1) {
    len = read(STDIN_FILENO, buf, BUFSIZ);
    if (len == 0) {
      break;
    }
    
    if (len == -1) {
      perror("read: ");
      exit(1);
    }

    chunk_lines = 0;
    chunk_chars = len;
    for (i=0; i<len; i++) {
      if (buf[i] == '\n') {
        chunk_lines++;
      }
    }
    total_lines += chunk_lines;
    total_chars += chunk_chars;
    gettimeofday(&tv_now, NULL);
    timersub(&tv_now, &tv_start, &tv_delta);
    total_seconds = tv_delta.tv_sec + (tv_delta.tv_usec / 1000000.0);
    if (total_seconds - last_seconds > 1) {
      fprintf(stderr, "%.0f lines / s\n", total_lines / total_seconds);
      last_seconds = total_seconds;
    }
  }
}
