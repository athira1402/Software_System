#include "bank.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void get_timestamp(char *buf, int len) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d",
             tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// Log a transaction
void log_transaction(const char *filename, struct Transaction *tr) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0) { perror("Transaction file"); return; }
    write(fd, tr, sizeof(*tr));
    close(fd);
}

// View transaction history
void view_transactions(const char *filename, int cust_id) {
    int fd = open(filename, O_RDONLY);
    if(fd < 0) { printf("No transaction history\n"); return; }

    struct Transaction tr;
    printf("===== Transaction History =====\n");
    while(read(fd, &tr, sizeof(tr)) == sizeof(tr)) {
        if(tr.cust_id == cust_id) {
            if(tr.to_id != 0) // example check
                printf("[%s] %s %.2f to Customer ID %d\n",
                       tr.timestamp, tr.type, tr.amount, tr.to_id);
            else
                printf("[%s] %s %.2f\n", tr.timestamp, tr.type, tr.amount);
        }
    }
    close(fd);
}
