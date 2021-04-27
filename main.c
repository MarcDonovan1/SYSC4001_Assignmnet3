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
//This will read the Database and find the value that the request needs to complete
char *readDB(struct mesg_server receievedInfo)
{
    FILE *fptr;
    char line[999];
    fptr = fopen("database.txt", "r");
    if (fptr == NULL)
    {
        printf("Unable to open files");
        exit(1);
    }

    while (fgets(line, sizeof(line), fptr))
    {
        char *token = strtok(line, " ");
        printf("%i", strcmp(token, receievedInfo.account.account_number));
        //Token is account number
        if (strcmp(token, receievedInfo.account.account_number) == 0)
        {
            //Token = pin
            token = strtok(NULL, " ");
            //Return pin
            if (receievedInfo.msg_type == PIN)
            {
                return token;
            }
            //Return balance
            else
            {
                //Toekn = balance
                token = strtok(NULL, " ");
                return token;
            }
        }
        token = NULL;
    }

    return NULL;
}

void updateDB(struct mesg_server receivedInfo)
{
    FILE *fptr;
    FILE *fTemp;
    int replace = 0;
    char line[99], c, information[99], reapeatLines[99];
    char number[20];
    fptr = fopen("database.txt", "a+");

    if (fptr == NULL)
    {
        printf("Unable to open files");
        exit(1);
    }
    int blah = atoi(receivedInfo.account.pin) + 1;
    char newPin[5];
    sprintf(newPin, "%d", blah);
    sprintf(number, "%.2f", receivedInfo.account.funds);
    snprintf(information, sizeof information, "%s %s %s\n", receivedInfo.account.account_number, newPin, number);
    //If the account is present already, setup a new file without the account to be updatted

    if (readDB(receivedInfo) != NULL)
    {
        fTemp = fopen("replace.txt", "a");
        if (fTemp == NULL)
        {
            printf("Unable to open files");
            exit(1);
        }
        while (fgets(line, sizeof(line), fptr))
        {
            char pin[3];
            char accountNumber[5];
            char funds[49];
            char *token = strtok(line, " ");

            //printf("\nThis is the accountNumber, pin, funds:%s %s %s  and the compare statement: %i", accountNumber,pin,funds, strcmp(accountNumber, receivedInfo.account.account_number));
            if (strcmp(token, receivedInfo.account.account_number) != 0)
            {
                sprintf(accountNumber, "%s", token);
                token = strtok(NULL, " ");
                sprintf(pin, "%s", token);
                token = strtok(NULL, " ");
                sprintf(funds, "%s", token);
                snprintf(reapeatLines, sizeof reapeatLines, "%s %s %s", accountNumber, pin, funds);
                fputs(reapeatLines, fTemp);
                memset(reapeatLines, 0, sizeof reapeatLines);
            }
        }
        //printf("\nInformation to be appended: %s", information);
        fputs(information, fTemp);
        fclose(fTemp);
        fclose(fptr);
        remove("database.txt");
        rename("replace.txt", "database.txt");
        //New account to be added
    }
    else
    {
        fputs(information, fptr);
        fclose(fptr);
    }
}

void db_Editor()
{
    struct mesg_account_creation account;
    struct mesg_server serverMsg;
    key_t key;
    int msgid;
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
        serverMsg.msg_type = UPDATE_DB;
        if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0) == -1)
        {
            perror("msgsnd: msgsnd faild");
            exit(1);
        }
    }
}

int checkAccount(struct pinVerify *accounts, char *attempt){

    return 0;
}

void db_Server()
{
    key_t key;
    int msgid;
    struct mesg_server account;
    struct mesg_server sendBack;
    struct mesg_account_creation holder;
    char *information;
    char temp[99];
    struct pinVerify accounts[15];
    int counter = 0;
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
            int pin;
            sscanf(information, "%d", &pin);
            pin--;
            sendBack.msg_type = OK;
            if (pin != atoi(account.account.pin))
            {
                if (checkAccount(accounts,account.account.account_number)==3){
                    sendBack.msg_type = ACCOUNT_BLOCKED;
                    //account.account.account_number.charAt(0);
                }
                else{
                    sendBack.msg_type = PIN_WRONG;
                }
                
            }
            if (msgsnd(msgid, &sendBack, sizeof(sendBack), 0) == -1)
            {
                perror("msgsnd: msgsnd faild");
                exit(1);
            }
        case BALANCE:
            information = readDB(account);
            snprintf(temp, sizeof temp, "%s", information);
            sendBack.msg_type = BALANCE;
            sendBack.account.funds = atof(temp);
            if (msgsnd(msgid, &sendBack, sizeof(sendBack), 0) == -1)
            {
                perror("msgsnd: msgsnd faild");
                exit(1);
            }
        case WITHDRAW:
            information = readDB(account);
            float funds;
            sscanf(information, "%f", &funds);
            if (funds > account.account.funds)
            {
                sendBack.msg_type = NSF;
            }
            else
            {
                sendBack.msg_type = FUNDS_OK;
                account.account.funds = account.account.funds - funds;
                updateDB(account);
            }
            if (msgsnd(msgid, &sendBack, sizeof(sendBack), 0) == -1)
            {
                perror("msgsnd: msgsnd faild");
                exit(1);
            }
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