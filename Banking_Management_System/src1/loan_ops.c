#include "bank.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void apply_loan(const char *filename, int cust_id, double amount) {
    if(amount <= 0) { printf("Invalid loan amount\n"); return; }

    struct LoanApplication loan;
    loan.cust_id = cust_id;
    loan.amount = amount;
    strcpy(loan.status, "Pending");
    get_timestamp(loan.timestamp, sizeof(loan.timestamp));

    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0) { perror("Loan file"); return; }

    write(fd, &loan, sizeof(loan));
    close(fd);
    printf("Loan application of %.2f submitted\n", amount);
}
