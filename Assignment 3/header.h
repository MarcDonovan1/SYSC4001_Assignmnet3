#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
struct mesg_account_creation
{
    int msg_type;
    char account_number[5];
    float funds;
    char pin[3];
};

struct mesg_server{
    int msg_type;
    struct mesg_account_creation account;
};

#define PIN_WRONG 1
#define BALANCE 2
#define WITHDRAW 3
#define NSF  4
#define FUNDS_OK  5
#define UPDATE_DB 6
#define OK 7
#define ACCOUNT_BLOCKED 8
#define PIN 9
#define ACCOUNT_OK 10