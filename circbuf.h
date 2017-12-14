/*
 * circbuf.h
 *
 *  Created on: Oct 8, 2017
 *      Author: Nathan
 */

#ifndef CIRCBUF_H_
#define CIRCBUF_H_

typedef struct {
    uint32_t* buffer;              // pointer to the base of the buffer in heap
    volatile uint32_t* head;       // pointer to first item
    volatile uint32_t* tail;       // pointer to last item
    volatile uint32_t num_items;  // number of items in buffer
    volatile uint32_t length;     // max number of items the buffer can hold
} CircBuf_t;

typedef enum {
    CB_NO_ERROR = 0,          // no error
    CB_INIT_ERROR,            // failed to initialize buffer
    CB_CLEAR_ERROR,           // failed to clear buffer
    CB_DELETE_ERROR,          // failed to delete buffer
    CB_ADD_ERROR,             // failed to add item
    CB_REMOVE_ERROR,          // failed to remove item
    CB_COPY_ERROR,            // failed to copy buffer
    ERROR                     // generic error
} CB_Status;

/* Setup the buffer in the heap and initialize the structure variables
 * @param buf = name of a circular buffer
 * @param length = initialization length
 * @ret CB_NO_ERROR if everything works, else CB_INIT_ERROR
 */
CB_Status CB_initialize_buffer(CircBuf_t** buf, uint32_t length);

/* Set the contents of a buffer to 0 and reset the buffer pointers
 * @param buf = name of a circular buffer
 * @ret CB_NO_ERROR if everything works, else CB_CLEAR_ERROR
 */
CB_Status CB_clear_buffer(CircBuf_t* buf);

/* Delete the buffer from dynamic memory
 * @param buf = name of a circular buffer
 * @ret CB_NO_ERROR if everything works, else CB_DELETE_ERROR
 */
CB_Status CB_delete_buffer(CircBuf_t* buf);

/* Checks if buffer is full
 * @param buf = name of a circular buffer
 * @ret nonzero value if buffer is full, else 0
 */
int8_t CB_is_buffer_full(CircBuf_t* buf);

/* Checks if buffer is empty
 * @param buf = name of a circular buffer
 * @ret nonzero value if buffer is empty, else 0
 */
int8_t CB_is_buffer_empty(CircBuf_t* buf);

/* Add a new item to a buffer
 * @param buf = name of a circular buffer
 * @param item = new item to add to buffer
 * @ret CB_NO_ERROR if everything works, else CB_ADD_ERROR
 */
CB_Status CB_add_item_to_buffer(CircBuf_t* buf, uint32_t item);

/* Remove an item from a buffer
 * @param buf = name of a circular buffer
 * @ret CB_NO_ERROR if everything works, else CB_REMOVE_ERROR
 */
uint32_t CB_remove_item_from_buffer(CircBuf_t* buf);

/* Copies contents of buffer to a char* without altering buffer
 * @param buf = name of a circular buffer
 * @param c = char* where contents will be stored
 * @ret CB_NO_ERROR if everything works, else CB_COPY_ERROR
 */
CB_Status CB_copy_buffer(CircBuf_t* buf, char* c);

#endif /* CIRCBUF_H_ */
