#include "bank.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// Authenticate customer
int authenticate(int dbfd, int id, char *password, struct Customer *cust) {
    lseek(dbfd, (id-1) * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));
    if(strcmp(cust->password, password) == 0) return id-1;
    return -1;
}

// View balance
void view_balance(int dbfd, int recno, struct Customer *cust) {
    lock_record(dbfd, recno, F_RDLCK);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));
    unlock_record(dbfd, recno);
    printf("Balance: %.2f\n", cust->balance);
}

// Deposit
void deposit(int dbfd, int recno, struct Customer *cust, double amount) {
    if(amount <= 0){ printf("Invalid deposit\n"); return; }

    lock_record(dbfd, recno, F_WRLCK);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));
    cust->balance += amount;
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    write(dbfd, cust, sizeof(*cust));
    unlock_record(dbfd, recno);

    struct Transaction tr = {cust->id, "Deposit", amount, 0, ""};
    get_timestamp(tr.timestamp, sizeof(tr.timestamp));
    log_transaction("transactions.dat", &tr);
    printf("Deposited %.2f\n", amount);
}

// Withdraw
void withdraw(int dbfd, int recno, struct Customer *cust, double amount) {
    lock_record(dbfd, recno, F_WRLCK);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));

    if(amount <= 0 || amount > cust->balance) {
        unlock_record(dbfd, recno);
        printf("Invalid/Insufficient\n");
        return;
    }

    cust->balance -= amount;
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    write(dbfd, cust, sizeof(*cust));
    unlock_record(dbfd, recno);

    struct Transaction tr = {cust->id, "Withdraw", amount, 0, ""};
    get_timestamp(tr.timestamp, sizeof(tr.timestamp));
    log_transaction("transactions.dat", &tr);
    printf("Withdrawn %.2f\n", amount);
}

// Transfer funds
void transfer_funds(int dbfd, int from_recno, struct Customer *from, int to_id, double amount) {
    struct Customer to;
    int to_recno = to_id - 1;

    int first = from_recno < to_recno ? from_recno : to_recno;
    int second = from_recno < to_recno ? to_recno : from_recno;

    lock_record(dbfd, first, F_WRLCK);
    lock_record(dbfd, second, F_WRLCK);

    lseek(dbfd, from_recno * sizeof(*from), SEEK_SET);
    read(dbfd, from, sizeof(*from));

    lseek(dbfd, to_recno * sizeof(to), SEEK_SET);
    read(dbfd, &to, sizeof(to));

    if(amount <= 0 || amount > from->balance) {
        unlock_record(dbfd, second);
        unlock_record(dbfd, first);
        printf("Invalid/Insufficient\n");
        return;
    }

    from->balance -= amount;
    to.balance += amount;

    lseek(dbfd, from_recno * sizeof(*from), SEEK_SET);
    write(dbfd, from, sizeof(*from));
    lseek(dbfd, to_recno * sizeof(to), SEEK_SET);
    write(dbfd, &to, sizeof(to));

    unlock_record(dbfd, second);
    unlock_record(dbfd, first);

    struct Transaction tr = {from->id, "Transfer", amount, to.id, ""};
    get_timestamp(tr.timestamp, sizeof(tr.timestamp));
    log_transaction("transactions.dat", &tr);
    printf("Transferred %.2f to Customer ID %d\n", amount, to.id);
}

// Change password
void change_password(int dbfd, int recno, struct Customer *cust, const char *new_pass) {
    if(strlen(new_pass) == 0 || strlen(new_pass) >= PASS_LEN) { printf("Invalid password\n"); return; }

    lock_record(dbfd, recno, F_WRLCK);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));
    strcpy(cust->password, new_pass);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    write(dbfd, cust, sizeof(*cust));
    unlock_record(dbfd, recno);
    printf("Password changed\n");
}

// Add feedback
void add_feedback(const char *filename, int cust_id, const char *feedback) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0){ perror("Feedback file"); return; }
    char buf[256];
    snprintf(buf, sizeof(buf), "Customer %d: %s\n", cust_id, feedback);
    write(fd, buf, strlen(buf));
    close(fd);
    printf("Feedback added\n");
}

