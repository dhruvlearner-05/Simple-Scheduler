#ifndef DUMMY_MAIN_H
#define DUMMY_MAIN_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int dummy_main(int argc, char **argv);

void handle_signal(int signal)
{
    if (signal == SIGCONT)
    {
        printf("Process continued.\n");
    }
}


int main(int argc, char **argv)
{
    struct sigaction sig1;

    sig1.sa_handler = handle_signal;
    sigemptyset(&sig1.sa_mask);
    sig1.sa_flags = 0;

    if (sigaction(SIGCONT, &sig1, NULL) == -1)
    {
        printf("Failed to add SIGCONT handler");
        exit(1);
    }


    // ignore SIGINT
    struct sigaction sig2;
    sig2.sa_handler = SIG_IGN;
    sigemptyset(&sig2.sa_mask);
    sig2.sa_flags = 0;

    if (sigaction(SIGINT, &sig2, NULL) == -1)
    {
        printf("Failed to set SIGINT to ignore\n");
        exit(1);
    }

    sigset_t signal_set;
    int signal;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGCONT);

    if (pthread_sigmask(SIG_BLOCK, &signal_set, NULL) != 0)
    {
        printf("Failed to block SIGCONT signal.\n");
        exit(1);
    }

    if (sigwait(&signal_set, &signal) != 0)
    {
        printf("Failed to wait for SIGCONT signal.\n");
        exit(1);
    }

    int ret = dummy_main(argc, argv);
    return ret;
}

#define main dummy_main

#endif
