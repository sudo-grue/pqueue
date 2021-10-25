/** @file pqueue.h
 * @brief thread-safe priority queue library for void pointers
 *        Requires pthread library
 */
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdbool.h>

/**
 * @brief Struct that defines pointer to head node
 */
typedef struct priority_queue pqueue;

/**
 * @brief Struct that defines pointer to node within priority queue
 */
typedef struct queue_node qnode;

/**
 * @brief Creates a priority queue
 * 
 * @return A newly malloc'd priority queue
 */
pqueue *pqueue_create();

/**
 * @brief Deletes a priority queue, freeing its resources (simple free only)
 * 
 * @param pq A queue to delete
 */
void pqueue_delete(pqueue *pq);

/**
 * @brief Inserts a given item of a specified priority into a queue
 * 
 * @param pq A priority queue
 * @param item An void pointer item to insert
 * @param priority The item's priority value
 */
void pqueue_insert(pqueue *pq, void *item, unsigned long priority);

/**
 * @brief Removes and returns the lowest-priority value from the queue
 *
 * @param pq Target priority queue
 * @return Address of stored item
 */
void *pqueue_extract(pqueue *pq);

/**
 * @brief Used to determine if pqueue is empty
 * 
 * @param pq A priority queue
 * @return True on empty, else False
 */
bool pqueue_is_empty(pqueue *pq);

#endif /* PQUEUE_H */

/*** END OF FILE ***/