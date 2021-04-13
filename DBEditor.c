

#include "header.h"

int checkInput(char string[], int length)
{
    int i;
    for (i = 0; string[i] != '\0'; i++)
        ;
    if (i != length)
    {
        return 0;
    }
    return 1;
}

/**
 * General format for data input
 * 12345 765 45.6
 * Account Number: 12345  
 * Pin: 765
 * Funds: 45.6
 * 
 */
int main()
{
    struct mesg_account_creation account;
    struct mesg_server serverMsg;
    key_t key;
    int msgid;
    //key = ftok("progfile", 65);
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    while (1)
    {

        printf("Enter Account Number, pin, then funds:");
        scanf("%s %s %f", account.account_number, account.pin, &account.funds);
        size_t size = sizeof(account.account_number) / sizeof(account.account_number[0]);
        while ((checkInput(account.account_number, 5) == 0) || (checkInput(account.pin, 3) == 0))
        {
            printf("Make sure your data is the correct length\n");
            printf("Enter Account Number, pin, then funds:");
            scanf("%s %s %f", account.account_number, account.pin, &account.funds);
            size = sizeof(account.account_number) / sizeof(account.account_number[0]);
        }

        printf("Account information\nAccount Number: %s \nPin: %s \nFunds: %f\n", account.account_number, account.pin, account.funds);
        serverMsg.account = account;
        serverMsg.msg_type = UPDATE_DB;
        serverMsg.account.msg_type=UPDATE_DB;
        if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
            perror("msgsnd: msgsnd faild");
            exit(1);
        }
    }
}