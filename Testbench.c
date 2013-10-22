
#include "functions.h"
#include "Queue.h"

Queue I2CQ;

void queueTest()
{
    Node Check;
    int error = 0;
    InitializeQueue(&I2CQ);
    error = addToQueue(&I2CQ, 8);
    error = addToQueue(&I2CQ, 9);
    error = addToQueue(&I2CQ, 10);
    error = addToQueue(&I2CQ, 11);
    error = addToQueue(&I2CQ, 12);
    error = addToQueue(&I2CQ, 13);
    error = addToQueue(&I2CQ, 14);
    error = addToQueue(&I2CQ, 15);
    error = addToQueue(&I2CQ, 16);
    error = addToQueue(&I2CQ, 17);
    error = addToQueue(&I2CQ, 18);
    error = addToQueue(&I2CQ, 19);
    error = addToQueue(&I2CQ, 20);
    error = addToQueue(&I2CQ, 21);
    error = addToQueue(&I2CQ, 22);
    error = addToQueue(&I2CQ, 23);
    error = addToQueue(&I2CQ, 24);
    error = addToQueue(&I2CQ, 25);
    error = addToQueue(&I2CQ, 26);
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = addToQueue(&I2CQ, 21);
    error = addToQueue(&I2CQ, 22);
    error = addToQueue(&I2CQ, 23);
    error = addToQueue(&I2CQ, 24);
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = addToQueue(&I2CQ, 23);
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
    error = addToQueue(&I2CQ, 23);
    error = popNode( &I2CQ, &Check );
    error = popNode( &I2CQ, &Check );
}