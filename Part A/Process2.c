#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include "shared_value.h"

void killChild(int sts)
{
    int id = getpid();
    switch (id)
    {
    case -1:
        perror("Termination failed failed");
        break;
    default:
        printf("\nProcess 2 has finished: PID = %d\n", id);
        kill(id, SIGKILL);
        break;
    }
}

int main()
{
    pid_t pid;
    char *message;
    char n;
    int exit_code;
    char *shared_memory;
    struct shared_use_st *shared_stuff;

    printf("fork program starting\n");
    pid = fork();

    int pidPrint = getpid();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
    case 0:
        //Child Process
        n = '2';
        //message = "I am Process 2";
        puts("This runs");
        (void)signal(SIGINT, killChild);
        break;
    default:
        //Main process
        n = '1';
        message = "I am Process 1";

        execl("./Process1", "./Process1", (char *)NULL);
        break;
    }
    puts("This is process 2");
    unsigned int shmid;
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    if (shmid == -1)
    {
        fprintf(stderr, "shmgasdasdet failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = (char *)shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    char input = '0';
    shared_stuff->changed = 1;
    while (1)
    {
        if (input == n)
        {
            puts("This is process 2");
        }
        else if (input == 'x'){
            puts("This quits");
            int id = getpid();
            printf("\nProcess 2 has finished: PID = %d\n", id);
            kill(id, SIGKILL);
        }
        scanf("%c", &input);
    }
}