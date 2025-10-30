#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/file.h>
#include "../include/client_io.h"
#include "../include/customer_ops.h"
#include "../include/transactions.h"
#include "../struct/customer.h"
#include "../struct/loan.h"
#include "../struct/transaction.h"
#include "../struct/feedback.h"
#include "../struct/employee.h"
#include "../struct/manager.h"
#include "../struct/admin.h"

#define PORT 8080
#define BUFFER_SIZE 1024

float get_balance(int uid)
{
    int fd = open(CUSTOMER_DB, O_RDONLY);
    if (fd < 0)
    {
        perror("Failed to open the file");
        return -1;
    }

    struct customer c;  
    int found = 0;      
    float balance = -1; 

    while (read(fd, &c, sizeof(struct customer)) == sizeof(struct customer))
    {
        printf("UserID : %d, Name: %s %s, Password: %s, Balance: %.2f, Loan: %.2f, Status: %s\n", c.userID, c.firstName, c.lastName, c.password, c.balance, c.loan, c.status);
        if (c.userID == uid)
        {
            printf("User ID %d found. Current balance: %.2f\n", c.userID, c.balance);
            balance = c.balance;
            found = 1;
            break; 
        }
    }

    close(fd); 

    if (!found)
    {
        printf("User ID %d not found.\n", uid);
        return -1; 
    }

    return balance;
}

int deposit(int uid, float amount)
{
    int fd = open(CUSTOMER_DB, O_RDWR); 
    if (fd < 0)
    {
        perror("Failed to open the file");
        return -1;
    }

    if (flock(fd, LOCK_EX) < 0)
    { 
        perror("Failed to acquire lock");
        close(fd);
        return -1;
    }

    struct customer c;
    int found = 0;
    off_t position; 
    while (read(fd, &c, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (c.userID == uid)
        {
            found = 1;
            c.balance += amount;                                    
            position = lseek(fd, -sizeof(struct customer), SEEK_CUR); 
            if (position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            if (write(fd, &c, sizeof(struct customer)) != sizeof(struct customer))
            {
                perror("Failed to write updated record");
                close(fd);
                return -1;
            } 
            log_transaction(uid, "Deposit", amount);
            break;
        }
    }

    flock(fd, LOCK_UN);
    close(fd);

    if (!found)
    {
        return -1;
    }

    return 0;
}

int withdraw(int uid, float amount)
{

    int fd = open(CUSTOMER_DB, O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open the file");
        return -1;
    }

    if (flock(fd, LOCK_EX) < 0)
    {
        perror("Failed to lock the file");
        close(fd);
        return -1;
    }

    struct customer c; 
    int found = 0;  
    off_t position;
    while (read(fd, &c, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (c.userID == uid)
        {
            if (c.balance < amount)
            {
                flock(fd, LOCK_UN);
                close(fd);
                return -1;
            }

            c.balance -= amount;

            position = lseek(fd, -sizeof(struct customer), SEEK_CUR); 
            if (position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }

            if (write(fd, &c, sizeof(struct customer)) != sizeof(struct customer))
            {
                perror("Failed to write updated record");
                flock(fd, LOCK_UN);
                close(fd);
                return -1;
            }
            found = 1;
            log_transaction(uid, "Withdraw", amount);
            break; 
        }
    }

    flock(fd, LOCK_UN);

    close(fd); 

    if (!found)
    {
        printf("User ID %d not found.\n", uid);
        return -1; 
    }

    return 0; 
}

int transfer(int from_uid, int to_uid, float amount)
{
    int fd = open(CUSTOMER_DB, O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open the file");
        return -1;
    }
    printf("\nFrom user: %d to user: %d", from_uid, to_uid);
    if (flock(fd, LOCK_EX) < 0)
    {
        perror("Failed to lock the file");
        close(fd);
        return -1;
    }

    struct customer c, from_customer, to_customer;
    int found_from = 0, found_to = 0;
    (void)found_from;
    off_t form_position, to_position;

    while (read(fd, &c, sizeof(struct customer)) == sizeof(struct customer))
    {
        printf("From UserID : %d, amount : %.2f\n", c.userID, c.balance);
        if (c.userID == from_uid)
        {
            if (c.balance < amount)
            {
                printf("Insufficient funds. Transfer aborted.\n");
                flock(fd, LOCK_UN); 
                close(fd);
                return -1;
            }
            found_from = 1;
            break;
        }
    }

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek failed");
        close(fd);
        return -1;
    }

    while (read(fd, &to_customer, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (to_customer.userID == to_uid)
        {
            printf("\nTo User ID %d found. Current balance: %.2f\n", to_customer.userID, to_customer.balance);
            found_to = 1;
            to_customer.balance += amount; 
            printf("\nUpdated balance: %.2f\n", to_customer.balance);
            to_position = lseek(fd, -sizeof(struct customer), SEEK_CUR); 
            if (to_position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            if (write(fd, &to_customer, sizeof(struct customer)) != sizeof(struct customer))
            {
                perror("Failed to write updated record");
                close(fd);
                return -1;
            }
            printf("To transfer successful. New balance: %.2f\n", to_customer.balance);
            log_transaction(to_uid, "Recieved Transfer", amount);
            break;
        }
    }

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek failed");
        close(fd);
        return -1;
    }

    if (found_to)
    {
        while (read(fd, &from_customer, sizeof(struct customer)) == sizeof(struct customer))
        {
            printf("Checking at From User ID %d found. Current balance: %.2f\n", from_customer.userID, from_customer.balance);
            if (from_customer.userID == from_uid)
            { 
                printf("From User ID %d found. Current balance: %.2f\n", from_customer.userID, from_customer.balance);
                from_customer.balance -= amount;
                printf("New balance after withdrawal: %.2f\n", from_customer.balance);

                form_position = lseek(fd, -sizeof(struct customer), SEEK_CUR); 
                if (form_position == -1)
                {
                    perror("lseek failed");
                    close(fd);
                    return -1;
                }

                if (write(fd, &from_customer, sizeof(struct customer)) != sizeof(struct customer))
                {
                    perror("Failed to write updated record");
                    flock(fd, LOCK_UN);
                    close(fd);
                    return -1;
                }
                printf("From transfer successful. New balance: %.2f\n", from_customer.balance);
                found_from = 1;
                log_transaction(from_uid, "Transfer", amount);
                break;
            }
        }
    }

    flock(fd, LOCK_UN);
    close(fd);

    printf("Transfer successful.\n");
    return 0;
}

int apply_loan(int uid, float amount)
{
    int fd = open("./data/loan.dat", O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (fd < 0)
    {
        perror("Failed to open loan data file");
        return -1;
    }

    if (flock(fd, LOCK_EX) < 0)
    {
        perror("Failed to acquire lock");
        close(fd);
        return -1;
    }

    struct loan new_loan;
    struct customer c;
    (void)c;
    c.loan = amount;
    new_loan.userID = uid;
    new_loan.amount = amount;
    strcpy(new_loan.status, "Pending");

    if (write(fd, &new_loan, sizeof(struct loan)) != sizeof(struct loan))
    {
        perror("Failed to write loan data");
        flock(fd, LOCK_UN);
        close(fd);
        return -1;
    }

    printf("Loan application submitted.\n");
    flock(fd, LOCK_UN);
    close(fd);

    return 0;
}

int change_cust_password(int uid, const char *new_password)
{
    int fd = open(CUSTOMER_DB, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening customer file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    struct customer c;
    int found = 0;
    off_t position;

    while (read(fd, &c, sizeof(struct customer)) == sizeof(struct customer))
    {
        if (c.userID == uid)
        {
            position = lseek(fd, -sizeof(struct customer), SEEK_CUR);
            if (position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            strncpy(c.password, new_password, sizeof(c.password) - 1);
            c.password[sizeof(c.password) - 1] = '\0';
            if (write(fd, &c, sizeof(struct customer)) != sizeof(struct customer))
            {
                perror("Failed to write updated record");
                flock(fd, LOCK_UN);
                close(fd);
                return -1;
            }
            found = 1;
            break;
        }
    }

    lock.l_type = F_UNLCK; 
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    if (!found)
    {
        printf("User ID %d not found.\n", uid);
        return -1;
    }

    printf("Password changed successfully.\n");
    return 0;
}

int add_feedback(int customerID, const char *message) 
{
    int fd = open(FEEDBACK_DB, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1) 
    {
        perror("Error opening feedback file");
        return -1;
    }

    if (flock(fd, LOCK_EX) < 0) 
    {
        perror("Error locking feedback file");
        close(fd);
        return -1;
    }

    static int feedback_counter = 0; 
    struct feedback new_feedback;
    new_feedback.feedbackID = ++feedback_counter; 
    new_feedback.customerID = customerID;
    strncpy(new_feedback.message, message, sizeof(new_feedback.message) - 1);
    new_feedback.message[sizeof(new_feedback.message) - 1] = '\0'; 
    strncpy(new_feedback.status, "Pending", sizeof(new_feedback.status) - 1);
    new_feedback.status[sizeof(new_feedback.status) - 1] = '\0'; 

    if (write(fd, &new_feedback, sizeof(struct feedback)) != sizeof(struct feedback)) 
    {
        perror("Failed to write feedback entry");
        flock(fd, LOCK_UN);
        close(fd);
        return -1;
    }

    flock(fd, LOCK_UN);
    close(fd);

    return 0; 
}

int view_transaction_history(int sock, int userID) 
{
    int fd = open(TRANSACTION_DB, O_RDONLY); 
    if (fd == -1) 
    {
        perror("Error opening transactions file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) 
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    struct transaction txn;
    int transaction_found = 0;
    char buffer[256]; 

    while (read(fd, &txn, sizeof(struct transaction)) == sizeof(struct transaction)) 
    {
        if (txn.customerID == userID) 
        {
            snprintf(buffer, sizeof(buffer), "Transaction ID: %d, Type: %s, Amount: %.2f, Timestamp: %s\n", txn.transactionID, txn.type, txn.amount, txn.timestamp);

            if (write(sock, buffer, strlen(buffer)) == -1) 
            {
                perror("Failed to send transaction data");
                flock(fd, LOCK_UN); 
                close(fd);
                return -1;
            }
            transaction_found = 1;
        }
    }

    if (!transaction_found) 
    {
        snprintf(buffer, sizeof(buffer), "No transaction history found for UserID: %d\n", userID);
        write(sock, buffer, strlen(buffer));
    }

    flock(fd, LOCK_UN);
    close(fd);

    return 0;  
}

void handle_logout(int client_sock)
{
    send(client_sock, "Logged Out...\n", strlen("Logged Out...\n"), 0);
    printf("User logged out.\n");
}