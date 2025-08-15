#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <sys/time.h>

typedef struct
{
    int pID;
    char command[256];
    int priority;
    struct timeval arrival_time;
    struct timeval end_time;
    long wait_time;
    long cycles;
} process;

typedef struct
{
    process processes[100];
    int front;
    int rear;
    int size;
} Queue;

void initializeQueue(Queue *queue);
bool is_empty(Queue *queue);
bool is_full(Queue *queue);
void enqueue(Queue *queue, process proc);
process dequeue(Queue *queue);
process peek(Queue *queue);
void sort(Queue *queue);
void clear(Queue *queue);

#endif
