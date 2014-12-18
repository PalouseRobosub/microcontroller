#include "System.h"

/*queue functions*/
Queue create_queue(uint8* buffer, uint buffer_size) {
    Queue queue;

    queue.buffer = buffer;
    queue.buffer_size = buffer_size;

    queue.QueueEnd = 0;
    queue.QueueStart = 0;
    queue.QueueLength = 0;

    return queue;
}

int enqueue(Queue* queue, uint8* data, uint data_size) {
    uint i, j;

    //disable interrupts
    asm volatile ("di");

    //check to see if there is room in the queue for the data
    if ((queue->buffer_size - queue->QueueLength) < data_size) {
        asm volatile("ei"); //re-enable interrupts
        return 1; //return error code
    }

    //copy the memory
    for (i = 0, j = queue->QueueEnd; i < data_size; ++i, ++j) {
        if (j == (queue->buffer_size)) //if we have reached end of buffer
        {
            j = 0; //wrap the buffer back around
        }

        queue->buffer[j] = data[i];
    }

    //update queue information
    queue->QueueEnd = j;
    queue->QueueLength += data_size;

    //enable interrupts
    asm volatile("ei");

    return 0; //return no error
}

int dequeue(Queue* queue, uint8* output_data, uint data_size) {
    uint i, j;

    //disable interrupts
    asm volatile ("di");

    //check if there is that much data in queue
    if (data_size > queue->QueueLength) {
        asm volatile("ei"); //re-enable interrupts
        return 1; //return error code
    }

    //copy the memory
    for (i = 0, j = queue->QueueStart; i < data_size; ++i, ++j) {
        if (j == (queue->buffer_size)) //if we have reached end of buffer
        {
            j = 0; //wrap the buffer back around
        }

        output_data[i] = queue->buffer[j];
    }

    queue->QueueStart = j;
    queue->QueueLength -= data_size;

    //enable interrupts
    asm volatile("ei");
    
    return 0; //return no error
}


