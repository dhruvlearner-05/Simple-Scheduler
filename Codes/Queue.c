#include <stdio.h>
#include <stdbool.h>
#include "Queue.h"

void initializeQueue(Queue *queue)
{
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

bool is_empty(Queue *queue)
{
    return queue->size == 0;
}

bool is_full(Queue *queue)
{
    return queue->size == 100;
}

void enqueue(Queue *queue, process proc)
{
    if (is_full(queue))
    {
        printf("Queue is full.\n");
        return;
    }
    queue->processes[queue->rear] = proc;
    queue->rear = (queue->rear + 1) % 100;
    queue->size++;
}

process dequeue(Queue *queue)
{
    process empty_process = {0};

    if (is_empty(queue))
    {
        printf("Queue is empty.\n");
        return empty_process;
    }

    process p = queue->processes[queue->front];
    queue->front = (queue->front + 1) % 100;
    queue->size--;

    return p;
}

process peek(Queue *queue)
{
    process empty_process = {0};

    if (is_empty(queue))
    {
        printf("Queue is empty.\n");
        return empty_process;
    }

    return queue->processes[queue->front];
}

void sort(Queue *queue)
{
    if (is_empty(queue))
        return;

    for (int i = 0; i < queue->size - 1; ++i)
    {
        for (int j = 0; j < queue->size - 1 - i; ++j)
        {
            int i1 = (queue->front + j) % 100;
            int i2 = (queue->front + j + 1) % 100;
            
            if (queue->processes[i1].priority > queue->processes[i2].priority)
            {
                process temp = queue->processes[i1];
                queue->processes[i1] = queue->processes[i2];
                queue->processes[i2] = temp;
            }
        }
    }
}

void clear(Queue *queue)
{
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}
