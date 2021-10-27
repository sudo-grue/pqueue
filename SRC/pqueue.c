#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"

typedef struct pqueue_node {
    void     *node_data;
    u_int16_t priority;
} qnode_t;

struct priority_queue {
    del_f           del;
    qnode_t        *back;
    qnode_t        *heap;
    u_int16_t       count;
    u_int16_t       capacity;
    pthread_mutex_t lock;
};

// Used after insertion to organize queue
//
static void heapify_up(u_int16_t position, qnode_t *heap);

// Used after extraction to reorganize queue
//
static void heapify_down(u_int16_t position, u_int16_t size, qnode_t *heap);


pqueue_t *pqueue_create(u_int16_t capacity, del_f del)
{
    pqueue_t *pq = malloc(sizeof(pqueue_t));
    if (!pq) {
        fprintf(stderr, "[!]ERR: Cannot allocate priority queue");
        return NULL;
    }

    pq->heap = calloc(capacity, sizeof(qnode_t));
    if (!pq->heap) {
        fprintf(stderr, "[!]ERR: Could not create heap. Try smaller capacity");
        free(pq);
        return NULL;
    }

    pq->count    = 0;
    pq->del      = del;
    pq->back     = pq->heap;
    pq->capacity = capacity;
    pthread_mutex_init(&pq->lock, NULL);

    return pq;
} /* pqueue_create() */

void pqueue_delete(pqueue_t *pq)
{
    if (!pq) {
        fprintf(stderr, "Could not destroy queue");
        return;
    }

    pthread_mutex_lock(&pq->lock);
    {
        if (!pqueue_is_empty(pq)) {
            qnode_t *temp = pq->heap;
            do {
                pq->del(temp->node_data);
                temp->node_data = NULL;
                ++temp;
            } while (temp != pq->back);
        }
        free(pq->heap);
        pq->back     = NULL;
        pq->heap     = NULL;
        pq->count    = 0;
        pq->capacity = 0;
    }
    pthread_mutex_unlock(&pq->lock);
    pthread_mutex_destroy(&pq->lock);
    free(pq);
} /* pqueue_delete() */

void pqueue_insert(pqueue_t *pq, void *item, u_int16_t priority)
{
    if (!pq | !pq->heap) {
        fprintf(stderr, "[!]ERR: Cannot open priority queue provided");
        return;
    }

    pthread_mutex_lock(&pq->lock);
    {
        if (pqueue_is_full(pq)) {
            fprintf(stderr, "[!]ERR: Queue is full");
            goto heapify_up_unlock;
        }
        pq->heap[pq->count].node_data = item;
        pq->heap[pq->count].priority  = priority;
        heapify_up(pq->count, pq->heap);
        pq->back++;
        pq->count++;
    }
    heapify_up_unlock:
    pthread_mutex_unlock(&pq->lock);

} /* pqueue_insert() */

static void heapify_up(u_int16_t position, qnode_t *heap)
{
    int32_t parent = (position - 1) / 2;
    if (parent < 0) {
        return;
    }

    qnode_t temp;
    if (heap[parent].priority > heap[position].priority) {
        temp           = heap[parent];
        heap[parent]   = heap[position];
        heap[position] = temp;
        heapify_up(parent, heap);
    }
} /* heapify_up() */


void *pqueue_extract(pqueue_t *pq)
{
    if (!pq | !pq->heap) {
        return NULL;
    }

    void *temp;
    pthread_mutex_lock(&pq->lock);
    {
        if (pqueue_is_empty(pq)) {
            fprintf(stderr, "[!]ERR: Failed extract, queue is empty");
            goto heapify_down_unlock;
        }

        temp = pq->heap[0].node_data;
        pq->count--;
        pq->back--;
        pq->heap[0] = pq->heap[pq->count];
        heapify_down(0, pq->count, pq->heap);
    }
    heapify_down_unlock:
    pthread_mutex_unlock(&pq->lock);
    return temp;
} /* pqueue_extract() */

// Used after extraction to reorganize queue
//
static void heapify_down(u_int16_t position, u_int16_t size, qnode_t *heap)
{
    u_int16_t lchild = 2 * position + 1;
    u_int16_t rchild = 2 * position + 2;

    if (lchild >= size) {
        return;
    }

    u_int16_t swap_idx = heap[lchild].priority <
                         heap[rchild].priority ? lchild : rchild;

    if (heap[position].priority > heap[swap_idx].priority) {
        qnode_t temp   = heap[swap_idx];
        heap[swap_idx] = heap[position];
        heap[position] = temp;

        heapify_down(swap_idx, size, heap);
    }
} /* heapify_down() */

bool pqueue_is_empty(pqueue_t *pq)
{
    return !pq || 0 == pq->count;
} /* pqueue_is_empty() */

bool pqueue_is_full(pqueue_t *pq)
{
    return pq->capacity == pq->count;
} /* pqueue_is_full() */

void pqueue_print(pqueue_t *pq)
{
    qnode_t p = pq->heap[0];
    char *c = p.node_data;
    fprintf(stderr, "%d:%c\n", p.priority, *c);
}

/*** END OF FILE ***/