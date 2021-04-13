#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
const int PIN = 0;
const int PIN_WRONG = 1;
const int BALANCE = 2;
const int WITHDRAW = 3;
const int NSF = 4;
const int FUNDS_OK = 5;
const int UPDATE_DB = 6;