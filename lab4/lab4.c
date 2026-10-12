// Nisal Panditha
// CMPT 201 Lab 4

#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void print_string(const char *str) { write(STDOUT_FILENO, str, strlen(str)); }

void print_number(uint64_t num) {
  char buf[32];
  int i = 0;

  do {
    buf[i++] = '0' + (num % 10);
    num /= 10;
  } while (num > 0);

  while (i > 0) {
    write(STDOUT_FILENO, &buf[--i], 1);
  }

  print_string("\n");
}

void print_address(void *ptr) {
  if (ptr == NULL) {
    print_string("(nil)\n");
    return;
  }

  char buf[2 * sizeof(uintptr_t)];
  uintptr_t address = (uintptr_t)ptr;
  int i = 0;

  do {
    int digit = address % 16;
    buf[i++] = digit < 10 ? '0' + digit : 'a' + digit - 10;
    address /= 16;
  } while (address > 0);

  print_string("0x");

  while (i > 0) {
    write(STDOUT_FILENO, &buf[--i], 1);
  }

  print_string("\n");
}

int main(void) {

  void *heap = sbrk(EXTRA_SIZE);

  if (heap == (void *)-1) {
    print_string("sbrk failed\n");
    return 1;
  }

  struct header *first = (struct header *)heap;

  struct header *second = (struct header *)((char *)heap + BLOCK_SIZE);

  first->size = BLOCK_SIZE;
  first->next = NULL;

  second->size = BLOCK_SIZE;
  second->next = first;

  size_t data_size = BLOCK_SIZE - sizeof(struct header);

  unsigned char *first_data = (unsigned char *)first + sizeof(struct header);

  unsigned char *second_data = (unsigned char *)second + sizeof(struct header);

  memset(first_data, 0, data_size);
  memset(second_data, 1, data_size);

  print_string("first block: ");
  print_address(first);

  print_string("second block: ");
  print_address(second);

  print_string("first block size: ");
  print_number(first->size);

  print_string("first block next: ");
  print_address(first->next);

  print_string("second block size: ");
  print_number(second->size);

  print_string("second block next: ");
  print_address(second->next);

  for (size_t i = 0; i < data_size; i++) {
    print_number(first_data[i]);
  }

  for (size_t i = 0; i < data_size; i++) {
    print_number(second_data[i]);
  }

  return 0;
}
