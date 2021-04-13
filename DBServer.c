#include "header.h"

/**
 *This is the main method for the DBServer
 *Below are the different codes 
 */
int main()
{
    key_t key;
    int msgid;
    struct mesg_server account;
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    while (1)
    {
        if (msgrcv(msgid, &account, sizeof(account), 0, 0) == -1)
        {
            perror("msgrcv: msgrcv failed");
            exit(1);
        }
        printf("%i\n", account.msg_type);
        switch (account.msg_type)
        {
        case PIN:
        case BALANCE:
        case WITHDRAW:
        case UPDATE_DB:
            FILE *fptr;
            FILE * fTemp;
            char buffer[999],oldInfo[99], newInfo[99];
            fptr = fopen("database.txt","r");
            fTemp = fopen("replace.tmp", "w");
            if (fptr==NULL || fTemp ==NULL){
                printf("Error");
                exit(1);
            } 
            
            while ((fgets()))
          

        }
    }
}