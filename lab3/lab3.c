// Nisal Panditha
// CMPT 201 Lab 3

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

void add_history(char *history[], char *input) {
  // Free the oldest input because it will be removed.
  free(history[0]);

  // Shift everything one position to the left.
  for (int i = 0; i < HISTORY_SIZE - 1; i++) {
    history[i] = history[i + 1];
  }

  // Store the newest input at the end.
  history[HISTORY_SIZE - 1] = input;
}

void print_history(char *history[]) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    if (history[i] != NULL) {
      printf("%s\n", history[i]);
    }
  }
}

void free_history(char *history[]) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    free(history[i]);
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {NULL};
  char *input = NULL;
  size_t size = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    ssize_t characters = getline(&input, &size, stdin);

    if (characters == -1) {
      free(input);
      break;
    }

    // Remove newline added by pressing Enter.
    if (characters > 0 && input[characters - 1] == '\n') {
      input[characters - 1] = '\0';
    }

    add_history(history, input);

    // history now owns this allocation.
    input = NULL;
    size = 0;

    // The newest input is always at index 4.
    if (strcmp(history[HISTORY_SIZE - 1], "print") == 0) {
      print_history(history);
    }
  }

  free_history(history);

  return 0;
}
