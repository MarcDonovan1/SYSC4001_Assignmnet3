#include "header.h"

char *readDB(struct mesg_server receievedInfo)
{
    FILE *fptr;
    int replace = 0;
    char buffer[999], oldInfo[99], newInfo[99];
    char information[30];
    char number[30];
    char space[] = " ";
    fptr = fopen("database.txt", "r");
    if (fptr == NULL)
    {
        printf("Unable to open files");
        exit(1);
    }

    while (fgets(number, sizeof(number), fptr) != NULL)
    {
        char *parts = strtok(number, space);

        //If we have found the account we want
        if (parts == receievedInfo.account.account_number)
        {
            //Pin request
            if (receievedInfo.msg_type == PIN)
           {
                return parts;
            }
            //Balance request
           char *parts = strtok(number, space);
            return parts;
        }
    }
    return NULL;
}

void updateDB(struct mesg_server receivedInfo)
{
    FILE *fptr;
    FILE *fTemp;
    int replace = 0;
    char buffer[999], oldInfo[99], newInfo[99];
    char information[90];
    char number[10];
    fptr = fopen("database.txt", "a");
    fTemp = fopen("replace.tmp", "w");
    if (fptr == NULL || fTemp == NULL)
    {
        printf("Unable to open files");
        exit(1);
    }
    int blah = atoi(receivedInfo.account.pin) + 1;
    char newPin[5];
    sprintf(newPin, "%d", blah);
    sprintf(number, "%.2f", receivedInfo.account.funds);
    strcpy(information, receivedInfo.account.account_number);
    strcat(information, " ");
    strcat(information, newPin);
    strcat(information, " ");
    strcat(information, number);
    strcat(information, " \n");
    fputs(information, fptr);
    fclose(fptr);
}

/**
 *This is the main method for the DBServer
 *Below are the different codes 
 */
int main()
{
    key_t key;
    int msgid;
    struct mesg_server account;
    struct mesg_server sendBack;
    char *information;
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    while (1)
    {
        if (msgrcv(msgid, &account, sizeof(account), 0, 0) == -1)
        {
            perror("msgrcv: msgrcv failed");
            exit(1);
        }
        switch (account.msg_type)
        {
        case PIN:
            information = readDB(account);
            sendBack.msg_type = OK;
            if (information != account.account.pin)
            {
                sendBack.msg_type = PIN_WRONG;
            }
            if (msgsnd(msgid, &sendBack, sizeof(sendBack), 0) == -1)
            {
                perror("msgsnd: msgsnd faild");
                exit(1);
            }
        case BALANCE:
            //strcat(information, readDB(account));
            //information = readDB(account);
            sendBack.msg_type = BALANCE;
            double amount = strtod(readDB(account),NULL);
            printf("%f",amount);
            //sendBack.account.funds = (float)strtod(readDB(account),NULL);
            if (msgsnd(msgid, &sendBack, sizeof(sendBack), 0) == -1)
            {
                perror("msgsnd: msgsnd faild");
                exit(1);
            }
        case WITHDRAW:
            break;
        case UPDATE_DB:
            updateDB(account);
        }
    }
}