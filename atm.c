#include "header.h"



int checkInput(char string[], int length);

struct mesg_account_creation requestAccountInfo();

void sendPinToDBServer(struct mesg_account_creation account, int msgid);
void sendBalanceToDBServer(struct mesg_account_creation account, int msgid);
int sendWithdrawToDBServer(struct mesg_account_creation account, int msgid);



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
        // requestAccountInfo calls scanf()'s to request info from the ATM user
        account = requestAccountInfo();
        sendPinToDBServer(account, msgid);

        // check if received message type is PIN_WRONG (value of 1)
        // if pin is wrong, try again
        while (msgrcv(msgid, &serverMsg, sizeof(serverMsg), 0, 0) == 1)
        {
            printf("Pin is wrong, please try again:");
            account = requestAccountInfo();
            sendPinToDBServer(account, msgid);
        }

        // Pin is not wrong, so continue with other cases
        switch(serverMsg.msg_type)
        {
            // case that the pin was wrong for a third time,
            // DBServer will reply with account_blocked
            case ACCOUNT_BLOCKED:
                printf("account is blocked");
                exit(1);

            case OK: 
            {                
                printf("please enter 0 to see your balance and 1 to withdraw:");
                int balanceOrWithdraw;
                scanf("%d",&balanceOrWithdraw);



                // -----              Customer has made it through successfully!              -----//
                // ----- They now have to select between getting their Balance or Withdrawing -----//

                // Option 1, BALANCE (value of 2):
                if (balanceOrWithdraw == 0)
                {
                    sendBalanceToDBServer(account, msgid);

                    msgrcv(msgid, &serverMsg, sizeof(serverMsg), 0, 0);
                    printf("Current Balance: %f", serverMsg.account.funds);
                }

                // Option 2, WITHDRAW (value of 3):
                else 
                {
                    sendWithdrawToDBServer(account, msgid);

                    // if FUNDS_OK (value of 5), 
                    if (msgrcv(msgid, &serverMsg, sizeof(serverMsg), 0, 0) == 5)
                    {
                    printf("Funds: %f", serverMsg.account.funds);
                    }
                    // else, not sufficient funds
                    else 
                    {
                        printf("insufficient funds to withdraw the requested amount");
                    }

                }
            }
        }
    }
}
    
int checkInput(char string[], int length)
{
    printf("%s\n", string);
    printf("%d\n", length);
    int i;
    for (i = 0; string[i] != '\0'; i++);
    if (i != length)
    {
        return 0;
    }
    return 1;
}

struct mesg_account_creation requestAccountInfo() 
{
    struct mesg_account_creation account;

    printf("requestAccountInfo\n");
    printf("Enter your account number (5 digits):");
    scanf("%s", account.account_number);

    printf("Enter your PIN number (3 digits):");
    scanf("%s", account.pin);

    // make sure account number length is 5 and pin length is 3
    while ((checkInput(account.account_number, 5) == 0) || (checkInput(account.pin, 3) == 0))
        {
            printf("requestAccountInfo - in while\n");
            printf("Make sure your data is the correct length\n");
            
            printf("Enter your account number:");
            scanf("%s", account.account_number);

            printf("Enter your PIN number:");
            scanf("%s", account.pin);
        }

    printf("leaving requestAccountInfo!\n\n");
    return account;
}

void sendPinToDBServer(struct mesg_account_creation account, int msgid)
{
    printf("sendPinToDBServer\n");
    struct mesg_server serverMsg;

    serverMsg.account = account;
    serverMsg.msg_type = PIN;
    if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
        perror("msgsnd: msgsnd failed");
        exit(1);
    }
}

void sendBalanceToDBServer(struct mesg_account_creation account, int msgid)
{
    struct mesg_server serverMsg;

    serverMsg.account = account;
    serverMsg.msg_type = BALANCE;
    if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
        perror("msgsnd: msgsnd failed");
        exit(1);
    }
}

int sendWithdrawToDBServer(struct mesg_account_creation account, int msgid)
{
    struct mesg_server serverMsg;

    // ADD THIS FOR WITHDRAW:
    // printf("Enter the amount you would like to withdraw:");
    // scanf("%s", account.some_new_withdrawal_amount_attribute);

    serverMsg.account = account;
    serverMsg.msg_type = WITHDRAW;
    if (msgsnd(msgid, &serverMsg, sizeof(serverMsg), 0)==-1){
        perror("msgsnd: msgsnd failed");
        exit(1);
    }

    // ADD THIS FOR WITHDRAW:
    // --- need a new attribute in the account struct --- //
    // ---   to pass the withdraw amount to be made   --- //
    // return account.some_new_withdrawal_amount_attribute;
}