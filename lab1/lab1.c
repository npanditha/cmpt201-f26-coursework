// Nisal Panditha
// CMPT 201 Lab 1

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  while (1) {
    char *line = NULL;
    size_t size = 0;
    char *saveptr;

    printf("Please enter some text: ");

    getline(&line, &size, stdin);

    char *ret = strtok_r(line, " ", &saveptr);

    printf("Tokens: \n");

    while (ret) {
      printf("  %s\n", ret);
      ret = strtok_r(NULL, " ", &saveptr);
    }

    free(line);
  }

  return 0;
}
