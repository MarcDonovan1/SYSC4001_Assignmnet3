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

struct mesg_account_creation requestAccountInfo() 
{
    struct mesg_account_creation account;

    printf("Enter your account number:");
    scanf("%s", account.account_number);

    printf("Enter your PIN number:");
    scanf("%s", account.pin);

    // make sure account number length is 5 and pin length is 3
    while ((checkInput(account.account_number, 5) == 0) || (checkInput(account.pin, 3) == 0))
        {
            printf("Make sure your data is the correct length\n");
            
            printf("Enter your account number:");
            scanf("%s", account.account_number);

            printf("Enter your PIN number:");
            scanf("%s", account.pin);
        }

    return account;
}

void sendPinToDBServer(struct mesg_account_creation account)
{
    serverMsg.account = account;
    serverMsg.msg_type = PIN;
    serverMsg.account.msg_type= PIN;
    if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
        perror("msgsnd: msgsnd failed");
        exit(1);
    }
}

void sendBalanceToDBServer(struct mesg_account_creation account)
{
    serverMsg.account = account;
    serverMsg.msg_type = BALANCE;
    serverMsg.account.msg_type= BALANCE;
    if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
        perror("msgsnd: msgsnd failed");
        exit(1);
    }
}

void sendWithdrawToDBServer(struct mesg_account_creation account)
{
    // ADD THIS FOR WITHDRAW:
    // printf("Enter the ammount you would like to withdraw:");
    // scanf("%s", account.withdraw);
    serverMsg.account = account;
    serverMsg.msg_type = WITHDRAW;
    serverMsg.account.msg_type= WITHDRAW;
    if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
        perror("msgsnd: msgsnd failed");
        exit(1);
    }
}


int main()
{
    struct mesg_account_creation account;
    struct mesg_server serverMsg;
    key_t key;
    int msgid;
    // int trialCounter = 0;

    //key = ftok("progfile", 65);
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    while (1)
    {

        account = requestAccountInfo();
        sendPinToDBServer(account);

        // check if received message type is 1, which corresponds to PIN_WRONG
        while (msgrcv(msgid, &serverMsg, sizeof(serverMsg), 0, 0) == 1)
        {
            printf("Pin is wrong, please try again:");
            account = requestAccountInfo();
            sendPinToDBServer(account);
        }

        // Pin is not wrong, continue with other cases
        switch (account.msg_type)
        {
        case ACCOUNT_BLOCKED:
            printf("account is blocked");
            exit(1);

        case ACCOUNT_OK: 
        int balanceOrWithdraw
        
        printf("please enter 0 to see your balance and 1 to withdraw:")
        scanf("%d",balanceOrWithdraw);

        // ----- Customer has made it through successfully!! They now have 2 options: -----//
        // Option 1, BALANCE (value of 2):
        if (balanceOrWithdraw == 0)
        {
            sendBalanceToDBServer(account);

            mmsgrcv(msgid, &serverMsg, sizeof(serverMsg), 0)
            printf("Current Balance: %f", serverMsg.account.funds);
            
        }

        // Option 2, WITHDRAW (value of 3):
        else 
        {
            sendWithdrawToDBServer(account);

            if (mmsgrcv(msgid, &serverMsg, sizeof(serverMsg), 0) == 2);
            printf("Funds: %f", serverMsg.account.funds);

            // else, not sufficient funds
            else 
            {
                printf("insufficient funds to withdraw the requested amount");
            }

        }
    }
}