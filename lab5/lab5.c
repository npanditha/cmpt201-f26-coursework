// Nisal Panditha
// CMPT 201 Lab 5

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size) {
      return free_list_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_size = UINT64_MAX;

  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size && free_list_ptr->size < best_size) {
      best_size = free_list_ptr->size;
      best_fit_id = free_list_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }

  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_size = 0;

  while (free_list_ptr != NULL) {
    if (free_list_ptr->size >= size && free_list_ptr->size > worst_size) {
      worst_size = free_list_ptr->size;
      worst_fit_id = free_list_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }

  return worst_fit_id;
}

int main(void) {
  struct header *free_block1 = malloc(sizeof(struct header));
  struct header *free_block2 = malloc(sizeof(struct header));
  struct header *free_block3 = malloc(sizeof(struct header));
  struct header *free_block4 = malloc(sizeof(struct header));
  struct header *free_block5 = malloc(sizeof(struct header));

  if (free_block1 == NULL || free_block2 == NULL || free_block3 == NULL || free_block4 == NULL ||
      free_block5 == NULL) {
    free(free_block1);
    free(free_block2);
    free(free_block3);
    free(free_block4);
    free(free_block5);
    return 1;
  }

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
Part 2: Coalescing Contiguous Free Blocks

Pseudocode:

1. When a block is freed, insert it into the free list.

2. Sort the free list by memory address if it is not
   already sorted.

3. Start at the first block in the free list.

4. While there is a next block:
   - Check if the current block ends exactly where
     the next block begins.
   - If they are adjacent:
       a. Combine their sizes.
       b. Remove the next block from the free list.
       c. Check the combined block against its new
          next block.
   - Otherwise:
       Move to the next block.

5. Continue until all adjacent free blocks have
   been combined.
*/
