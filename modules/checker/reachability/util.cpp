
#include "util.h"

#include <signal.h>

#include <cstdio>
#include <cstdlib>

int popen2(const char *cmdline, struct popen2 *childinfo) {
  pid_t p;
  int pipe_stdin[2], pipe_stdout[2];

  if (pipe(pipe_stdin)) return -1;
  if (pipe(pipe_stdout)) return -1;

  p = fork();
  if (p < 0) return p; /* Fork failed */
  if (p == 0) {        /* child */
    close(pipe_stdin[1]);
    dup2(pipe_stdin[0], 0);
    close(pipe_stdout[0]);
    dup2(pipe_stdout[1], 1);
    execl("/bin/sh", "sh", "-c", cmdline, 0);
    perror("execl");
    exit(99);
  }

  // THESE TWO LINES ARE ESSENTIAL AND WERE ABSENT IN THE ORIGINAL SOURCE.
  // 2 HOURS DOWN THE DRAIN.
  close(pipe_stdin[0]);
  close(pipe_stdout[1]);

  childinfo->child_pid = p;
  childinfo->to_child = pipe_stdin[1];
  childinfo->from_child = pipe_stdout[0];

  if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
  	perror(0);
  	exit(1);
  }

  return 0;
}