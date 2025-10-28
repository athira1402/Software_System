#include "bank.h"

void get_timestamp(char *buf, int len) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d",
             tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void lock_record(int fd, int recno, short type) {
    struct flock lock = {0};
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = recno * sizeof(struct Customer);
    lock.l_len = sizeof(struct Customer);
    fcntl(fd, F_SETLKW, &lock);
}

void unlock_record(int fd, int recno) {
    struct flock lock = {0};
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = recno * sizeof(struct Customer);
    lock.l_len = sizeof(struct Customer);
    fcntl(fd, F_SETLK, &lock);
}

int authenticate(int dbfd, int id, char *password, struct Customer *cust) {
    struct Customer temp;
    int recno = 0;
    lseek(dbfd, 0, SEEK_SET);
    while(read(dbfd, &temp, sizeof(temp)) == sizeof(temp)) {
        if(temp.id == id && strcmp(temp.password, password) == 0) {
            *cust = temp;
            return recno;
        }
        recno++;
    }
    return -1;
}

void deposit(int dbfd, int recno, struct Customer *cust, double amount) {
    if(amount <= 0) return;
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
}

void withdraw(int dbfd, int recno, struct Customer *cust, double amount) {
    lock_record(dbfd, recno, F_WRLCK);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));
    if(amount <= 0 || amount > cust->balance) {
        unlock_record(dbfd, recno);
        return;
    }
    cust->balance -= amount;
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    write(dbfd, cust, sizeof(*cust));
    unlock_record(dbfd, recno);

    struct Transaction tr = {cust->id, "Withdraw", amount, 0, ""};
    get_timestamp(tr.timestamp, sizeof(tr.timestamp));
    log_transaction("transactions.dat", &tr);
}

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
}

void change_password(int dbfd, int recno, struct Customer *cust, const char *new_pass) {
    if(strlen(new_pass) == 0 || strlen(new_pass) >= PASS_LEN) return;
    lock_record(dbfd, recno, F_WRLCK);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    read(dbfd, cust, sizeof(*cust));
    strcpy(cust->password, new_pass);
    lseek(dbfd, recno * sizeof(*cust), SEEK_SET);
    write(dbfd, cust, sizeof(*cust));
    unlock_record(dbfd, recno);
}

void log_transaction(const char *filename, struct Transaction *tr) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0) return;
    write(fd, tr, sizeof(*tr));
    close(fd);
}

void view_transactions(const char *filename, int cust_id) {
    int fd = open(filename, O_RDONLY);
    if(fd < 0) return;
    struct Transaction tr;
    while(read(fd, &tr, sizeof(tr)) == sizeof(tr)) {
        if(tr.cust_id == cust_id) {
            if(tr.to_id)
                printf("[%s] %s %.2f to ID %d\n", tr.timestamp, tr.type, tr.amount, tr.to_id);
            else
                printf("[%s] %s %.2f\n", tr.timestamp, tr.type, tr.amount);
        }
    }
    close(fd);
}

void apply_loan(const char *filename, int cust_id, double amount) {
    if(amount <= 0) return;
    struct LoanApplication loan = {cust_id, amount, "Pending", ""};
    get_timestamp(loan.timestamp, sizeof(loan.timestamp));
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0) return;
    write(fd, &loan, sizeof(loan));
    close(fd);
}

void add_feedback(const char *filename, int cust_id, const char *feedback) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0) return;
    char buf[256];
    snprintf(buf, sizeof(buf), "Customer %d: %s\n", cust_id, feedback);
    write(fd, buf, strlen(buf));
    close(fd);
}