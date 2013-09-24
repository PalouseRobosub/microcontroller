
#include "functions.h"
#include "Queue.h"

void queueTest()
{
    Node Check;
    InitializeQueue(&I2CQ);
    addToQueue(&I2CQ, 8);
    addToQueue(&I2CQ, 9);
    addToQueue(&I2CQ, 10);
    freeNode( &I2CQ, &Check );
    freeNode( &I2CQ, &Check );
    freeNode( &I2CQ, &Check );
    freeNode( &I2CQ, &Check );
    freeNode( &I2CQ, &Check );
}