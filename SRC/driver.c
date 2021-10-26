#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pqueue.h"

#define TESTS 10

int main()
{
    srand(time(NULL));
    printf("\nInserting %d random numbers with random priority\n", TESTS);

    pqueue_t *minque = pqueue_create(TESTS, free);
    if (!minque) {
        fprintf(stderr, "Could not create minque\n");
        return 1;
    }

    for (int i = 0; i < 5; ++i) {
        char *key = malloc(sizeof(char));
        int priority = rand() % 128;
        *key = rand() % 26 + 64;
        pqueue_insert(minque, key, priority);
    }

    for (int i = 0; i < 5; ++i) {
        char *key = malloc(sizeof(char));
        int priority = 0;
        *key = rand() % 26 + 64;
        fprintf(stderr, "%c ", *key);
        pqueue_insert(minque, key, priority);
    }
    fprintf(stderr, "\n");

    while (!pqueue_is_empty(minque)) {
        pqueue_print(minque);
        int *key = pqueue_extract(minque);
        free(key);
    }
    pqueue_delete(minque);
}