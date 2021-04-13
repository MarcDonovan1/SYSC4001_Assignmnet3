#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void killParent()
{
    int stat_val;
    pid_t child_pid = wait(&stat_val);
    
}

int main()
{
    //(void)signal(SIGINT, killParent);
    int input = 1;
    int n = 1;
    puts("This is process 1");
    int stat_val;
    pid_t child_pid = wait(&stat_val);
    int id = getpid();
    printf("\nProcess 1 has finished: PID = %d\n", id);
    kill(id, SIGKILL);
}