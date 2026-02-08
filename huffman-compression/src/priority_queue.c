#include "priority_queue.h"

// Create empty priority queue
PriorityQueue* createPriorityQueue() {
    PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

// Insert node into priority queue (min heap)
void insertNode(PriorityQueue *pq, Node *node) {
    if (pq->size >= 256) return;
    
    int i = pq->size;
    pq->nodes[i] = node;
    pq->size++;
    
    // Bubble up to maintain min heap property
    while (i > 0 && pq->nodes[i]->frequency < pq->nodes[(i - 1) / 2]->frequency) {
        Node *temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        pq->nodes[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// Extract node with minimum frequency
Node* extractMin(PriorityQueue *pq) {
    if (pq->size == 0) return NULL;
    
    Node *min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;
    
    // Bubble down to maintain min heap property
    int i = 0;
    while (1) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < pq->size && pq->nodes[left]->frequency < pq->nodes[smallest]->frequency)
            smallest = left;
        
        if (right < pq->size && pq->nodes[right]->frequency < pq->nodes[smallest]->frequency)
            smallest = right;
        
        if (smallest == i) break;
        
        Node *temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[smallest];
        pq->nodes[smallest] = temp;
        i = smallest;
    }
    
    return min;
}

// Check if priority queue is empty
int isEmptyPQ(PriorityQueue *pq) {
    return pq->size == 0;
}

// Free priority queue
void freePriorityQueue(PriorityQueue *pq) {
    free(pq);
}
