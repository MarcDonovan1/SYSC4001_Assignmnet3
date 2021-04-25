#include <stdio.h>
#include "header.h"
// DB can be a file or be in memory for this exercise

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

char *readDB(struct mesg_server receievedInfo)
{
    FILE *fptr;
    int replace = 0;
    char line[999];
    char space[] = " ";
    fptr = fopen("database.txt", "r");
    if (fptr == NULL)
    {
        printf("Unable to open files");
        exit(1);
    }

    while (fgets(line, sizeof(line), fptr))
    {
        printf("y");
        char *token = strtok(line, " ");
        printf("%i",strcmp(token, receievedInfo.account.account_number));
        if (strcmp(token, receievedInfo.account.account_number) == 0)
        {
            token = strtok(NULL, " ");
            //Return balance
            if (receievedInfo.msg_type==BALANCE){
                return token;
            }
            //Return Pin
            else{
                token = strtok(NULL, " ");
                return token;
            }
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

void db_Editor()
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
        while ((checkInput(account.account_number, 5) == 0) || (checkInput(account.pin, 3) == 0))
        {
            printf("Make sure your data is the correct length\n");
            printf("Enter Account Number, pin, then funds:");
            scanf("%s %s %f", account.account_number, account.pin, &account.funds);
        }

        printf("Account information\nAccount Number: %s \nPin: %s \nFunds: %f\n", account.account_number, account.pin, account.funds);
        serverMsg.account = account;
        serverMsg.msg_type = PIN;
        serverMsg.account.msg_type = UPDATE_DB;
        if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0) == -1)
        {
            perror("msgsnd: msgsnd faild");
            exit(1);
        }
    }
}

void db_Server()
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
            printf("%s", information);
            sendBack.msg_type = OK;
            // if (information != account.account.pin)
            // {
            //     sendBack.msg_type = PIN_WRONG;
            // }
            if (msgsnd(msgid, &sendBack, sizeof(sendBack), 0) == -1)
            {
                perror("msgsnd: msgsnd faild");
                exit(1);
            }
        case BALANCE:
            information = readDB(account);
            sendBack.msg_type = BALANCE;
            sendBack.account.funds=atof(information);
            //double amount = atof(readDB(account));
            //printf("%f",amount);
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

int main()
{

    pid_t pid = fork();

    if (pid == 0)
        db_Editor();

    if (pid > 0)
        db_Server();
}
