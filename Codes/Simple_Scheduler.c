#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <poll.h>
#include <string.h>

#include "message.h"
#include "Queue.h"

int ncpu, tslice;

char *pipeName = "/tmp/Simple_Scheduler_pipe";

Queue readyQueue;
Queue runningQueue;
Queue completedQueue;
Queue failedQueue;

// add tslice to all processes in ready queue
void addWaitTime()
{
    for (int i = readyQueue.front; i < readyQueue.rear; i++)
    {
        readyQueue.processes[i].wait_time += tslice;
    }
}


// stop all running processes
void stopRunningProcesses()
{
    for (int i = runningQueue.front; i < runningQueue.rear; i++)
    {
        process p = dequeue(&runningQueue);

        int pID = p.pID;

        // sending SIGSTOP signal to running process
        if (kill(pID, SIGSTOP) == -1)
        {
            if (errno == ESRCH)
            {
                if (gettimeofday(&p.end_time, NULL) != 0)
                {
                    printf("Failed to get time.\n");
                    exit(0);
                }

                // adding to completed queue
                enqueue(&completedQueue, p);
            }
            else
            {
                printf("Failed to stop process %d.\n", pID);
                exit(0);
            }
        }
        else
        {
            // adding to ready queue
            enqueue(&readyQueue, p);
        }
    }

    // Reset runningQueue
    clear(&runningQueue);
}


// receiving message from shell
void receiveMessage()
{
    int c;
    do
    {
        c = -1;
        const int fd = open(pipeName, O_RDONLY | O_NONBLOCK);

        if (fd == -1)
        {
            printf("Failed to open pipe.\n");
            exit(0);
        }

        struct pollfd pfd;
        pfd.fd = fd;
        pfd.events = POLLIN;

        // checking if there is data to read or not
        int result = poll(&pfd, 1, 1);

        if (result == -1)
        {
            printf("Failed to poll.\n");
            close(fd);
            exit(0);
        }
        else if (pfd.revents & POLLIN)
        {
            struct message msg;

            ssize_t bytes_read = read(fd, &msg, sizeof(struct message));

            if (bytes_read > 0)
            {
                process p;

                p.pID = msg.pID;
                p.priority = msg.priority;
                p.cycles = 0;
                p.wait_time = 0;
                strcpy(p.command, msg.command);

                c = 0;

                if (gettimeofday(&p.arrival_time, NULL) != 0)
                {
                    printf("Failed to get time.\n");
                    exit(0);
                }

                // adding to ready queue
                enqueue(&readyQueue, p);
            }
        }

        close(fd);

    } while (c != -1);
}


// sleep scheduler for tslice milliseconds
void sleepTslice()
{
    struct timespec slice;

    slice.tv_sec = tslice / 1000;
    slice.tv_nsec = (tslice % 1000) * 1000000;

    if (nanosleep(&slice, NULL) == -1)
    {
        printf("Failed to sleep.\n");
        exit(0);
    }
}


// execute at most ncpu processes from ready queue
void execute()
{
    sort(&readyQueue);

    while (runningQueue.size != ncpu && !is_empty(&readyQueue))
    {
        process p = dequeue(&readyQueue);

        // sending SIGCONT signal
        int result = kill(p.pID, SIGCONT);

        if (result == -1)
        {
            if (errno != ESRCH)
            {
                printf("Failed to send SIGCONT");
            }

            // adding process to failed queue from ready queue
            enqueue(&failedQueue, p);
        }
        else
        {
            // adding process to running queue from ready queue
            p.cycles++;
            enqueue(&runningQueue, p);
        }
    }

    addWaitTime();
    sleepTslice();
}


void printHistory()
{
    printf("\n\n%s\n", "-------------------------------------------------------------------------------------");
    printf("%s\n", "                                       History                                           ");

    if (!is_empty(&failedQueue))
    {
        printf("\n\n%s\n", "-------------------------------------------------------------------------------------");
        printf("%s\n", "                                   Failed Commands                                           ");
    }

    for (int i = failedQueue.front; i < failedQueue.rear; ++i)
    {
        printf("%s\n", "-------------------------------------------------------------------------------------");

        process p = dequeue(&failedQueue);

        char time_str[13];
        struct tm *tm_info;

        tm_info = localtime(&p.arrival_time.tv_sec);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        snprintf(time_str + 8, sizeof(time_str) - 8, ":%03ld", p.arrival_time.tv_usec / 1000);

        printf("\n");
        printf("\tP_ID : %d\n", p.pID);
        printf("\tCommand : %s\n", p.command);
        printf("\tArrival time : %s\n", time_str);
        printf("\tExecution Cycles : %ld\n", p.cycles);
    }

    double avg_wait_time = 0;
    double avg_exec_time = 0;
    double cnt = 0;

    if (!is_empty(&completedQueue))
    {
        printf("\n\n%s\n", "-------------------------------------------------------------------------------------");
        printf("%s\n", "                                 Completely Executed Commands                                           ");
    }

    for (int i = completedQueue.front; i < completedQueue.rear; ++i)
    {
        process p = dequeue(&completedQueue);

        cnt++;
        printf("%s\n", "-------------------------------------------------------------------------------------");

        char time_str[13];
        struct tm *tm_info;

        tm_info = localtime(&p.arrival_time.tv_sec);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        snprintf(time_str + 8, sizeof(time_str) - 8, ":%03ld", p.arrival_time.tv_usec / 1000);

        printf("\n");
        printf("\tP_ID : %d\n", p.pID);
        printf("\tCommand : %s\n", p.command);
        printf("\tArrival time : %s\n", time_str);
        printf("\tWaiting time : %ld milliseconds\n", p.wait_time);

        tm_info = localtime(&p.end_time.tv_sec);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        snprintf(time_str + 8, sizeof(time_str) - 8, ":%03ld", p.end_time.tv_usec / 1000);

        long duration = p.cycles * tslice;

        printf("\tCompletion time : %s\n", time_str);
        printf("\tExecution Duration : %ld milliseconds\n", duration);
        printf("\tExecution Cycles : %ld\n", p.cycles);

        avg_wait_time += p.wait_time;
        avg_exec_time += duration;
    }

    if (cnt != 0)
    {
        printf("%s\n", "-------------------------------------------------------------------------------------");
        printf("\tAverage waiting time : %.6f second\n", avg_wait_time / cnt);
        printf("\tAverage execution time : %.6f second\n", avg_exec_time / cnt);
        printf("%s\n", "-------------------------------------------------------------------------------------");
    }
    

}


// SIGTERM signal handler
void sigterm_handler(int signum)
{

    while (!is_empty(&runningQueue) || !is_empty(&readyQueue)){
        execute();
        stopRunningProcesses();
    }

    printHistory();

    exit(0);
}

// handle signals
void handle_signals()
{
    // handling SIGINT signal to be ignored
    struct sigaction sig;
    sig.sa_handler = SIG_IGN;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;

    if (sigaction(SIGINT, &sig, NULL) == -1)
    {
        printf("Failed to add SIGINT handler.\n");
        exit(0);
    }

    // handling SIGTERM signal
    struct sigaction sa;
    sa.sa_handler = sigterm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        printf("Failed to add SIGTERM handler.\n");
        exit(0);
    }
}



int main(int argc, char const *argv[])
{
    ncpu = atoi(argv[1]);
    tslice = atoi(argv[2]);

    handle_signals();

    initializeQueue(&readyQueue);
    initializeQueue(&runningQueue);
    initializeQueue(&completedQueue);
    initializeQueue(&failedQueue);

    while (true)
    {
        receiveMessage();
        execute();
        stopRunningProcesses();
    }

    return 0;
}
