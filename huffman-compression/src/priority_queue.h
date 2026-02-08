#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "huffman.h"

typedef struct {
    Node *nodes[256];
    int size;
} PriorityQueue;

// Priority Queue operations
PriorityQueue* createPriorityQueue();
void insertNode(PriorityQueue *pq, Node *node);
Node* extractMin(PriorityQueue *pq);
int isEmptyPQ(PriorityQueue *pq);
void freePriorityQueue(PriorityQueue *pq);

#endif
