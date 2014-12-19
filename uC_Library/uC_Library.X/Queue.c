#include "Queue.h"

Queue create_queue(uint8* buffer, uint buffer_size){
	Queue queue;

	queue.buffer = buffer;
	queue.buffer_size = buffer_size;

	queue.QueueEnd = 0;
	queue.QueueStart = 0;
	queue.numStored = 0;

	return queue;
}
int enqueue(Queue* queue, uint8* data, uint data_size) {
	uint i, j;

	//check to see if there is room in the queue for the data
	if ((queue->buffer_size - queue->numStored) < data_size)
	{
		return 1;
	}

	//copy the memory
	for (i = 0, j = queue->QueueEnd; i < data_size; ++i, ++j)
	{
		if (j == (queue->buffer_size)) //if we have reached end of buffer
		{
			j = 0; //wrap the buffer back around
		}

		queue->buffer[j] = data[i];
	}

	//update queue information
	queue->QueueEnd = j;
	queue->numStored += data_size;

	return 0;
}
int dequeue(Queue* queue, uint8* output_data, uint data_size) {
	uint i, j;

	//disable interrupts

	//check if there is that much data in queue
	if (data_size > queue->numStored) {
		//re-enable interrupts
		return 1;
	}

	//copy the memory
	for (i = 0, j = queue->QueueStart; i < data_size; ++i, ++j)
	{
		if (j == (queue->buffer_size)) //if we have reached end of buffer
		{
			j = 0; //wrap the buffer back around
		}

		output_data[i] = queue->buffer[j];
	}

	queue->QueueStart = j;
	queue->numStored -= data_size;

	//disable interrupts
	return 0;
}