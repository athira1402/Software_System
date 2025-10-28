#ifndef BANK_H
#define BANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define NAME_LEN 50
#define PASS_LEN 20
#define EMP_USER_LEN 20
#define EMP_PASS_LEN 20

#define ROLE_CUSTOMER 1
#define ROLE_EMPLOYEE 2
#define ROLE_MANAGER  3
#define ROLE_ADMIN    4

#define CUSTOMER_DB "data/customers.dat"
#define EMPLOYEE_DB "data/employees.dat"
#define MANAGER_DB  "data/managers.dat"
#define ADMIN_DB    "data/admins.dat"

struct Customer {
    int id;
    char username[NAME_LEN];
    char password[PASS_LEN];
    double balance;
};

struct Transaction {
    int cust_id;
    char type[20];
    double amount;
    int to_id;
    char timestamp[30];
};

struct LoanApplication {
    int cust_id;
    double amount;
    char status[20];
    char timestamp[30];
};

struct Employee {
    int emp_id;
    char username[EMP_USER_LEN];
    char password[EMP_PASS_LEN];
    char role[20];   // e.g. "Manager", "Clerk"
};


void get_timestamp(char *buf, int len);
int authenticate(int dbfd, int id, char *password, struct Customer *cust);
void lock_record(int fd, int recno, short type);
void unlock_record(int fd, int recno);
void deposit(int dbfd, int recno, struct Customer *cust, double amount);
void withdraw(int dbfd, int recno, struct Customer *cust, double amount);
int transfer_funds(int dbfd, int from_recno, struct Customer *from, int to_id, double amount);
void change_password(int dbfd, int recno, struct Customer *cust, const char *new_pass);
void log_transaction(const char *filename, struct Transaction *tr);
void view_transactions(int client_sock, const char *filename, int cust_id);
int apply_loan(const char *filename, int cust_id, double amount);
void add_feedback(const char *filename, int cust_id, const char *feedback);
int is_logged_in(int user_id,const char *session_file);
void add_session(int user_id,const char *session_file);
void remove_session(int user_id,const char *session_file);
void start_customer_client();
void start_employee_server();
void start_employee_client();
void start_manager_server();
void start_admin_server();
void start_customer_server();
int add_customer(int dbfd, const char *name, const char *password, double balance);

#endif