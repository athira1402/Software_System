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
#include "../include/client_io.h"
#include "../include/customer_ops.h"
#include "../include/transactions.h"
#include "../include/employee_ops.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int update_customer_status(int custID, const char *action) 
{
    FILE *file = fopen(CUSTOMER_DB, "rb+");
    if (!file) 
    {
        perror("Error opening customer data");
        return 0;
    }

    struct customer cust;
    while (fread(&cust, sizeof(struct customer), 1, file)) 
    {
        if (cust.userID == custID) 
        {
            if (strcmp(action, "ACTIVATE") == 0) 
            {
                strcpy(cust.status, "Active");
            } 
            else 
            {
                strcpy(cust.status, "Inactive");
            }

            fseek(file, -sizeof(struct customer), SEEK_CUR); 
            fwrite(&cust, sizeof(struct customer), 1, file);
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0;
}

int assign_loan_to_employee(int empID, int loanID) 
{
    FILE *file = fopen(EMPLOYEE_DB, "rb+");
    if (!file) 
    {
        perror("Error opening employee data");
        return 0;
    }

    struct employee emp;
    while (fread(&emp, sizeof(struct employee), 1, file)) 
    {
        if (emp.employeeID == empID && emp.loan_count < 10) 
        {
            emp.assigned_loans[emp.loan_count++] = loanID;

            fseek(file, -sizeof(struct employee), SEEK_CUR);
            fwrite(&emp, sizeof(struct employee), 1, file);  
            fclose(file);
            return 1; 
        }
    }

    fclose(file);  
    return 0;  
}

int change_mng_password(int uid, const char *new_password)
{
    int fd = open(MANAGER_DB, O_RDWR);
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

    struct manager c;
    int found = 0;
    off_t position;

    while (read(fd, &c, sizeof(struct manager)) == sizeof(struct manager))
    {
        if (c.managerID == uid)
        {
            position = lseek(fd, -sizeof(struct manager), SEEK_CUR);
            if (position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            strncpy(c.password, new_password, sizeof(c.password) - 1);
            c.password[sizeof(c.password) - 1] = '\0';
            if (write(fd, &c, sizeof(struct manager)) != sizeof(struct manager))
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