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
#define PORT 8080
#define BUFFER_SIZE 1024
#include "../include/customer_ops.h"
#include "../include/transactions.h"
#include "../include/client_io.h"

int add_customer(int uid, const char *fname, const char *lname, const char *pwd, float bal) 
{
    int fd = open(CUSTOMER_DB, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) 
    {
        perror("Failed to open the customer file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = 0; 

    if (fcntl(fd, F_SETLKW, &lock) == -1) 
    {
        perror("Failed to lock the file");
        close(fd);
        return -1;
    }

    struct customer new_customer;
    new_customer.userID = uid;
    strncpy(new_customer.firstName, fname, sizeof(new_customer.firstName) - 1);
    new_customer.firstName[sizeof(new_customer.firstName) - 1] = '\0';
    strncpy(new_customer.lastName, lname, sizeof(new_customer.lastName) - 1);
    new_customer.lastName[sizeof(new_customer.lastName) - 1] = '\0';
    strncpy(new_customer.password, pwd, sizeof(new_customer.password) - 1);
    new_customer.password[sizeof(new_customer.password) - 1] = '\0';
    new_customer.balance = bal;
    new_customer.loan = 0;
    strncpy(new_customer.status, "Active", sizeof(new_customer.status) - 1);
    new_customer.status[sizeof(new_customer.status) - 1] = '\0';

    if (write(fd, &new_customer, sizeof(struct customer)) != sizeof(struct customer)) 
    {
        perror("Failed to write new customer to the file");
        fcntl(fd, F_UNLCK, &lock); 
        close(fd);
        return -1;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    printf("New customer (ID: %d) added successfully.\n", uid);
    return 0; 
}

int fetch_assigned_loan_details(int *loan_ids, int loan_count, char *details) 
{
    struct loan ln;
    FILE *loan_file = fopen(LOAN_DB, "rb");
    if (!loan_file) {
        perror("Error opening loan data file");
        return -1;
    }

    char temp[100];
    strcpy(details, "Assigned Loan Details:\n");

    for (int i = 0; i < loan_count; i++) 
    {
        rewind(loan_file);

        while (fread(&ln, sizeof(struct loan), 1, loan_file)) 
        {
            if (ln.userID == loan_ids[i]) 
            {
                sprintf(temp, "Loan ID: %d, Amount: %.2f, Status: %s\n",
                        ln.userID, ln.amount, ln.status);
                strcat(details, temp);  
                break;
            }
        }
    }

    fclose(loan_file);
    return 0;
}

int view_employee_loans(int employee_id, char *details) 
{
    struct employee emp;
    FILE *emp_file = fopen(EMPLOYEE_DB, "rb");
    if (!emp_file) {
        perror("Error opening employee data file");
        return -1;
    }

    while (fread(&emp, sizeof(struct employee), 1, emp_file)) 
    {
        if (emp.employeeID == employee_id) 
        {
            fclose(emp_file);
            return fetch_assigned_loan_details(emp.assigned_loans, emp.loan_count, details);
        }
    }

    fclose(emp_file);
    return -1; 
}

int update_loan_status(int loan_id, const char *new_status) 
{
    struct loan ln;
    int loan_found = 0;

    FILE *loan_file = fopen(LOAN_DB, "rb+");
    if (!loan_file) {
        perror("Error opening loan data file");
        return -1;
    }

    while (fread(&ln, sizeof(struct loan), 1, loan_file)) 
    {
        if (ln.userID == loan_id) 
        {
            loan_found = 1;
            strcpy(ln.status, new_status); 
            fseek(loan_file, -sizeof(struct loan), SEEK_CUR);
            fwrite(&ln, sizeof(struct loan), 1, loan_file);
            break;
        }
    }

    fclose(loan_file);
    return loan_found ? ln.amount : -1; 
}

int credit_to_customer(int cust_id, float amount) 
{
    struct customer cust;
    int customer_found = 0;

    FILE *customer_file = fopen(CUSTOMER_DB, "rb+");
    if (!customer_file) 
    {
        perror("Error opening customer data file");
        return -1;
    }

    while (fread(&cust, sizeof(struct customer), 1, customer_file)) 
    {
        if (cust.userID == cust_id) 
        {
            customer_found = 1;
            cust.balance += amount;
            cust.loan += amount;
            fseek(customer_file, -sizeof(struct customer), SEEK_CUR);
            fwrite(&cust, sizeof(struct customer), 1, customer_file);
            break;
        }
    }

    fclose(customer_file);
    return customer_found ? 0 : -1;
}

void view_customer_transactions(int sock) 
{
    char buffer[BUFFER_SIZE];
    int cust_id;

    if (read(sock, buffer, sizeof(buffer) - 1) <= 0) 
    {
        perror("Error reading User ID from client");
        close(sock);
        return;
    }
    sscanf(buffer, "%d", &cust_id);

    FILE *file = fopen("./data/transaction.data", "rb");
    if (!file) 
    {
        perror("Error opening transactions file");
        strcpy(buffer, "Error opening transactions file.\n");
        write(sock, buffer, strlen(buffer));
        return;
    }

    struct transaction txn;
    int found = 0;
    char response[BUFFER_SIZE] = "";

    while (fread(&txn, sizeof(struct transaction), 1, file)) 
    {
        if (txn.customerID == cust_id) 
        {
            found = 1;
            char temp[200];
            snprintf(temp, sizeof(temp), "ID: %d | Type: %s | Amount: %.2f | Date: %s\n", txn.transactionID, txn.type, txn.amount, txn.timestamp);
            strcat(response, temp);
        }
    }
    fclose(file);

    if (!found) 
    {
        strcpy(response, "No transactions found for this customer.\n");
    }
    printf("%s",response);
    write(sock, response, strlen(response));
}

int change_emp_password(int uid, const char *new_password)
{
    int fd = open(EMPLOYEE_DB, O_RDWR);
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

    struct employee c;
    int found = 0;
    off_t position;

    while (read(fd, &c, sizeof(struct employee)) == sizeof(struct employee))
    {
        if (c.employeeID == uid)
        {
            position = lseek(fd, -sizeof(struct employee), SEEK_CUR);
            if (position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            strncpy(c.password, new_password, sizeof(c.password) - 1);
            c.password[sizeof(c.password) - 1] = '\0';
            if (write(fd, &c, sizeof(struct employee)) != sizeof(struct employee))
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

void delete_customer(int custID, int sock) 
{
    char buffer[BUFFER_SIZE];
    int found = 0;

    FILE *fp = fopen(CUSTOMER_DB, "rb");
    FILE *temp = fopen(TEMP_CUST_DB, "wb");

    if (!fp || !temp) {
        perror("File error");
        strcpy(buffer, "Error accessing customer data.\n");
        write(sock, buffer, strlen(buffer));
        close(sock);
        return;
    }

    struct customer c;

    while (fread(&c, sizeof(struct customer), 1, fp)) 
    {
        if (c.userID == custID) 
        {
            found = 1; 
        } 
        else 
        {
            fwrite(&c, sizeof(struct customer), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) 
    {
        remove(CUSTOMER_DB);
        rename(TEMP_CUST_DB, CUSTOMER_DB);
        strcpy(buffer, "Customer deleted successfully.\n");
    } 
    else 
    {
        remove(TEMP_CUST_DB);
        strcpy(buffer, "Customer not found.\n");
    }

    write(sock, buffer, strlen(buffer));
}
