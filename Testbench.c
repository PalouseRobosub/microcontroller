
#include "functions.h"

void queueTest()
{
    Node Check;
    InitializeQueue(I2CQueue);
    addToQueue(I2CQueue, 8);
    addToQueue(I2CQueue, 9);
    addToQueue(I2CQueue, 10);
    freeNode( I2CQueue, &Check );

    addToQueue(I2CQueue, 11);
}