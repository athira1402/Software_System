#ifndef TRANSACTION
#define TRANSACTION

struct transaction {
    int transactionID;
    int customerID;
    char type[20];
    float amount;
    char timestamp[30];
};

#endif