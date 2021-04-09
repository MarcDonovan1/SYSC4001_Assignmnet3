#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
        message = "I am Process 2";
        //(void)signal(SIGINT, killChild);
        break;
    default:
        //Main process
        n = '1';
        message = "I am Process 1";

        execl("./Process1", "./Process1", (char *)NULL);
        break;
    }
    char input = '2';
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