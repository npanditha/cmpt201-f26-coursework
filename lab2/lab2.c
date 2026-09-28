// Nisal Panditha
// CMPT 201 Lab 2

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char *input = NULL;
  size_t size = 0;

  while (1) {
    printf("Enter programs to run.\n");
    printf("> ");

    if (getline(&input, &size, stdin) == -1) {
      free(input);
      return 1;
    }

    // Remove the \n added by getline()
    input[strcspn(input, "\n")] = '\0';

    pid_t pid = fork();

    if (pid < 0) {
      perror("fork");
      free(input);
      return 1;
    }

    if (pid == 0) {
      // Child process
      execlp(input, input, NULL);

      // Only runs if execlp() fails
      printf("Exec failure\n");
      free(input);
      exit(1);
    } else {
      // Parent process
      if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        free(input);
        return 1;
      }
    }
  }

  free(input);
  return 0;
}
