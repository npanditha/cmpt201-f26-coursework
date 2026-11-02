// Nisal Panditha
// CMPT 201 lab6

// Corrected example_1.c

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)
{
  if (!(expr)) {
    fprintf(stderr, "Assertion failed: %s\n", #expr);
    exit(1);
  }
}

#define TEST(expr)
{
  if (!(expr)) {
    fprintf(stderr, "Test failed: %s\n", #expr);
    exit(1);
  } else {
    printf("Test passed: %s\n", #expr);
  }
}

typedef struct node {
  uint64_t data;
  struct node *next;
} node_t;

node_t *head = NULL;

void insert_sorted(uint64_t data) {
  node_t *new_node = malloc(sizeof(node_t));
  ASSERT(new_node != NULL);

  new_node->data = data;
  new_node->next = NULL;

  if (head == NULL || data < head->data) {
    new_node->next = head;
    head = new_node;
  } else {
    node_t *curr = head;

    while (curr->next != NULL && curr->next->data <= data) {
      curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
  }
}

int index_of(uint64_t data) {
  node_t *curr = head;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

int main() {
  insert_sorted(1);
  insert_sorted(2);
  insert_sorted(5);
  insert_sorted(3);

  TEST(index_of(3) == 2);

  insert_sorted(0);
  insert_sorted(4);

  TEST(index_of(4) == 4);

  return 0;
}

// Corrected example_2.c

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)
{
  if (!(expr)) {
    fprintf(stderr, "Assertion failed: %s\n", #expr);
    fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__);
    exit(1);
  }
}

#define TEST(expr)
{
  if (!(expr)) {
    fprintf(stderr, "Test failed: %s\n", #expr);
    exit(1);
  }
}

typedef struct node {
  uint64_t data;
  struct node *next;
} node_t;

typedef struct info {
  uint64_t sum;
} info_t;

node_t *head = NULL;
info_t info = {0};

// Calculate the sum of all nodes in the linked list
uint64_t sum_list() {
  uint64_t sum = 0;
  node_t *curr = head;

  while (curr != NULL) {
    sum += curr->data;
    curr = curr->next;
  }

  return sum;
}

// Insert a new node while keeping the list sorted
void insert_sorted(uint64_t data) {
  node_t *new_node = malloc(sizeof(node_t));
  ASSERT(new_node != NULL);

  new_node->data = data;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
  } else if (data < head->data) {
    new_node->next = head;
    head = new_node;
  } else {
    node_t *curr = head;
    node_t *prev = NULL;

    while (curr != NULL) {
      if (data < curr->data) {
        break;
      }

      prev = curr;
      curr = curr->next;
    }

    prev->next = new_node;
    new_node->next = curr;
  }

  // Update the stored sum
  info.sum += data;

  // Check that the stored sum matches the actual list sum
  ASSERT(info.sum == sum_list());
}

// Find the index of a value in the list
int index_of(uint64_t data) {
  node_t *curr = head;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

int main() {
  insert_sorted(1);
  insert_sorted(3);
  insert_sorted(5);
  insert_sorted(2);

  TEST(info.sum == 1 + 3 + 5 + 2);
  TEST(index_of(2) == 1);

  // Final assertion to verify the sum
  ASSERT(info.sum == sum_list());

  return 0;
}
