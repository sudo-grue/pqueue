/** @file pqueue.h
 * @brief thread-safe minimum priority queue (minque) library for void*
 *        - Requires pthread library
 */
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdbool.h>

/**
 * @brief Struct that defines pointer to min-node, pthread lock,
 *        and del_f that knows how to delete custom void*
 */
typedef struct priority_queue pqueue_t;

/**
 * @brief User provided function to delete void* without memory leaks
 */
typedef void (*del_f)(void *data);

/**
 * @brief Creates a priority queue
 * 
 * Provides user with the option of a custom delete function pending what
 * datatype is stored within the queue.
 *  - Built-in datatype : NULL
 *  - "Simple" alloc'd  : free()
 *  - Custom Structure  : User Provided Function
 * 
 * The purpose of this option, is so the library does not need to know
 * anything about the user defined data structure to successfully delete
 *
 * @param capacity Maximum amount of elemnts that can be held in queue 
 * @param del Function to delete void*. Pass NULL if not wanted
 * @return pqueue_t* On success, NULL on failure
 */
pqueue_t *pqueue_create(u_int16_t capacity, del_f del);

/**
 * @brief Deletes a priority queue, freeing resources used and
 *        deleting any remaining items without warning
 * 
 * @param pq The queue to delete
 */
void pqueue_delete(pqueue_t *pq);

/**
 * @brief Inserts void* of a specified priority into minque
 * 
 * @param pq Priority queue to query
 * @param item Void* element to insert
 * @param priority Item's priority (0-4,294,967,295) where 0 is top priority
 */
void pqueue_insert(pqueue_t *pq, void *item, u_int16_t priority);

/**
 * @brief Removes and returns the lowest-priority value from the queue
 *
 * @param pq Target priority queue
 * @return Address of stored item
 */
void *pqueue_extract(pqueue_t *pq);

/**
 * @brief Used to determine if pqueue_t is empty
 * 
 * @param pq A priority queue
 * @return True on empty, else False
 */
bool pqueue_is_empty(pqueue_t *pq);

/**
 * @brief Used to determine if pqueue_t is full
 * 
 * @param pq A priority queue
 * @return True on full, else False
 */
bool pqueue_is_full(pqueue_t *pq);

void pqueue_print(pqueue_t *pq);

#endif /* PQUEUE_H */
/*** END OF FILE ***/