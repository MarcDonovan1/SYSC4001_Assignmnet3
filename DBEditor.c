#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct mesg_account_creation
{
    char account_number[5];
    float funds;
    char pin[3];
};

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

    while (1)
    {
        printf("Enter Account Number, pin, then funds:");
        scanf("%s %s %f", account.account_number, account.pin, &account.funds);
        size_t size = sizeof(account.account_number) / sizeof(account.account_number[0]);
        printf("%i", checkInput(account.account_number, 5));
        while ((checkInput(account.account_number, 5) == 0)||(checkInput(account.pin, 3) == 0))
        {
            printf("Make sure your data is the correct length: Account Number\n");
            printf("Enter Account Number, pin, then funds:");
            scanf("%s %s %f", account.account_number, account.pin, &account.funds);
            size = sizeof(account.account_number) / sizeof(account.account_number[0]);
        }

        printf("Account information\nAccount Number: %s \nPin: %s \nFunds: %f\n", account.account_number, account.pin, account.funds);
    }
}