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
        printf("Process 1 has finished: PID = %d\n", id);
        kill(id, SIGKILL);
        break;
    }
    
}

void killParent(){
    int stat_val;
    pid_t child_pid = wait(&stat_val);
    int id = getpid();
    printf("Process 2 has finished: PID = %d\n", id);
    kill(id, SIGKILL);

    
}

int main()
{   
    pid_t pid;
    char *message;
    int n;
    int exit_code;
    printf("fork program starting\n");
    pid = fork();
    
     int pidPrint = getpid();
    printf("%d \n", pidPrint);
    switch (pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
    case 0:
        //n = 5;
        message = "I am Process 1";
        (void)signal(SIGINT, killChild);
        break;
    default:
        //n = 3;
        message = "I am Process 2";
        (void)signal(SIGINT, killParent);
        break;
    }
    while (1)
    {
        puts(message);
        
        sleep(1);
    }
}