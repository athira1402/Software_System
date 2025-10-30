#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/file.h>
#include "../struct/customer.h"
#include "../struct/loan.h"
#include "../struct/transaction.h"
#include "../struct/feedback.h"
#include "../struct/employee.h"
#include "../struct/manager.h"
#include "../struct/admin.h"

void log_transaction(int customerID, const char *type, float amount) 
{
    struct transaction txn;
    static int transaction_counter = 0;

    txn.transactionID = ++transaction_counter;
    txn.customerID = customerID;
    strncpy(txn.type, type, sizeof(txn.type) - 1);
    txn.type[sizeof(txn.type) - 1] = '\0'; 
    txn.amount = amount;

    time_t now = time(NULL);
    strftime(txn.timestamp, sizeof(txn.timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    int fd = open("./data/transaction.data", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) 
    {
        perror("Failed to open transaction log");
        return;
    }

    if (flock(fd, LOCK_EX) < 0) {
        perror("Failed to lock the transaction log");
        close(fd);
        return;
    }

    if (write(fd, &txn, sizeof(struct transaction)) != sizeof(struct transaction)) 
    {
        perror("Failed to write transaction to log");
    }

    flock(fd, LOCK_UN);
    close(fd);
}