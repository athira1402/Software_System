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

#include "../include/customer_ops.h"
#include "../include/transactions.h"
#include "../include/employee_ops.h"
#include "../include/manager_ops.h"
#include "../include/admin_ops.h"
#include "../include/customer_client.h"
#include "../include/employee_client.h"
#include "../include/manager_client.h"
#include "../include/admin_client.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_employee_requests(int sock, const char *user_id)
{
    int id = atoi(user_id);
    char buffer[BUFFER_SIZE];
    while (1)
    {
        
        int bytes_read = read(sock, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            perror("Client disconnected or read error");
            close(sock);
            return;
        }
        buffer[bytes_read] = '\0';

        if (strcmp(buffer, "ADD_CUSTOMER") == 0)
        {
            int uid;
            char fname[30], lname[30], pwd[50], status[20];
            float bal, loan;
            (void)loan;
            (void)status;

            if (read(sock, buffer, sizeof(buffer) - 1) <= 0) {
                perror("Error reading User ID from client");
                close(sock);
                return;
            }
            sscanf(buffer, "%d", &uid);

            bytes_read = read(sock, fname, sizeof(fname) - 1);
            fname[bytes_read] = '\0';
            fname[strcspn(fname, "\n")] = '\0';


            bytes_read = read(sock, lname, sizeof(lname) - 1);
            lname[bytes_read] = '\0';
            lname[strcspn(lname, "\n")] = '\0';

            bytes_read = read(sock, pwd, sizeof(pwd) - 1);
            pwd[bytes_read] = '\0';
            pwd[strcspn(pwd, "\n")] = '\0';

            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%f", &bal);

            if (add_customer(uid, fname, lname, pwd, bal) == 0) {
                strcpy(buffer, "Customer added successfully.\n");
            } else {
                strcpy(buffer, "Failed to add customer.\n");
            }
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "MANAGE_CUSTOMER") == 0) 
        {
            int uid;
            char new_fname[50], new_lname[50], new_status[20], new_password[50], buffer[BUFFER_SIZE];
            float new_balance, new_loan;
            int option, bytes_read;

            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%d", &uid);

            struct customer c;
            int fd = open("./data/customer.dat", O_RDWR);
            if (fd == -1) {
                perror("Error opening customer file");
                strcpy(buffer, "Error: Unable to open customer file.\n");
                write(sock, buffer, strlen(buffer));
                return;
            }

            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 0;

            if (fcntl(fd, F_SETLK, &lock) == -1) {
                perror("Error locking customer file");
                strcpy(buffer, "Error: Unable to lock the file.\n");
                write(sock, buffer, strlen(buffer));
                close(fd);
                return;
            }

            int found = 0;
            off_t position;

            while (read(fd, &c, sizeof(struct customer)) == sizeof(struct customer)) 
            {
                if (c.userID == uid) 
                {
                    found = 1;
                    position = lseek(fd, -sizeof(struct customer), SEEK_CUR);
                    break;
                }
            }

            if (!found) 
            {
                snprintf(buffer, sizeof(buffer), "Customer with ID %d not found.\n", uid);
                write(sock, buffer, strlen(buffer));
                flock(fd, F_UNLCK);
                close(fd);
                return;
            }

            snprintf(buffer, sizeof(buffer), "ID: %d\nName: %s %s\nBalance: %.2f\nLoan: %.2f\nStatus: %s\n", c.userID, c.firstName, c.lastName, c.balance, c.loan, c.status);
            write(sock, buffer, strlen(buffer));

            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            option = atoi(buffer);

            switch (option) 
            {
                case 1:
                    read(sock, new_fname, sizeof(new_fname));
                    new_fname[sizeof(new_fname) - 1] = '\0';
                    size_t len = strlen(new_fname);
                    if (len > 0 && new_fname[len - 1] == '\n') 
                    {
                        new_fname[len - 1] = '\0';
                    }
                    memset(buffer, 0, sizeof(buffer));

                    read(sock, new_lname, sizeof(new_lname));
                    new_lname[sizeof(new_lname) - 1] = '\0';
                    len = strlen(new_lname);
                    if (len > 0 && new_lname[len - 1] == '\n') 
                    {
                        new_lname[len - 1] = '\0';
                    }
                    memset(buffer, 0, sizeof(buffer));

                    bytes_read = read(sock, buffer, sizeof(buffer) - 1);
                    buffer[bytes_read] = '\0';
                    sscanf(buffer, "%f", &new_balance);
                    memset(buffer, 0, sizeof(buffer));

                    bytes_read = read(sock, buffer, sizeof(buffer) - 1);
                    buffer[bytes_read] = '\0';
                    sscanf(buffer, "%f", &new_loan);
                    memset(buffer, 0, sizeof(buffer));

                    read(sock, new_status, sizeof(new_status));
                    new_status[sizeof(new_status) - 1] = '\0';
                    len = strlen(new_status);
                    if (len > 0 && new_status[len - 1] == '\n') 
                    {
                        new_status[len - 1] = '\0';
                    }

                    strncpy(c.firstName, new_fname, sizeof(c.firstName) - 1);
                    strncpy(c.lastName, new_lname, sizeof(c.lastName) - 1);
                    c.balance = new_balance;
                    c.loan = new_loan;
                    strncpy(c.status, new_status, sizeof(c.status) - 1);

                    lseek(fd, position, SEEK_SET);
                    if (write(fd, &c, sizeof(struct customer)) != sizeof(struct customer)) 
                    {
                        perror("Failed to write updated customer record");
                        strcpy(buffer, "Error: Failed to update customer.\n");
                        write(sock, buffer, strlen(buffer));
                    } 
                    else 
                    {
                        strcpy(buffer, "Customer details updated successfully.\n");
                        write(sock, buffer, strlen(buffer));
                    }
                    break;

                case 2: 
                    delete_customer(uid, sock);
                    break;

                case 3: 
                    bytes_read = read(sock, new_password, sizeof(new_password) - 1);
                    new_password[bytes_read] = '\0';
                    len = strlen(new_password);
                    if (len > 0 && new_password[len - 1] == '\n') 
                    {
                        new_password[len - 1] = '\0';
                    }

                    strncpy(c.password, new_password, sizeof(c.password) - 1);

                    lseek(fd, position, SEEK_SET);
                    if (write(fd, &c, sizeof(struct customer)) != sizeof(struct customer)) 
                    {
                        perror("Failed to reset password");
                        strcpy(buffer, "Error: Failed to reset password.\n");
                        write(sock, buffer, strlen(buffer));
                    } 
                    else 
                    {
                        strcpy(buffer, "Password reset successfully.\n");
                        write(sock, buffer, strlen(buffer));
                    }
                    break;

                case 4:
                    strcpy(buffer, "Exiting customer management.\n");
                    write(sock, buffer, strlen(buffer));
                    break;

                default:
                    strcpy(buffer, "Invalid option.\n");
                    write(sock, buffer, strlen(buffer));
                    break;
            }
            flock(fd, F_UNLCK);
            close(fd);
        }
        else if (strcmp(buffer, "VIEW_LOAN_APPL") == 0)
        {
            printf("Processing VIEW_LOAN_APPL request\n");
            char loan_details[BUFFER_SIZE];
            int result = view_employee_loans(id, loan_details);

            if (result == 0) 
            {
                write(sock, loan_details, strlen(loan_details));
            } 
            else 
            {
                strcpy(buffer, "Failed to retrieve loan applications.\n");
                write(sock, buffer, strlen(buffer));
            }
        }
        else if (strcmp(buffer, "APP/REJ_LOANS") == 0)
        {
            printf("Processing APP/REJ_LOANS request\n");
            bytes_read = read(sock, buffer, BUFFER_SIZE);
            buffer[bytes_read] = '\0';
            int loan_id = atoi(buffer);

            bytes_read = read(sock, buffer, BUFFER_SIZE);
            buffer[bytes_read] = '\0';
            char new_status[20];
            strcpy(new_status, buffer);

            float loan_amount = update_loan_status(loan_id, new_status);
            if (loan_amount == -1) 
            {
                strcpy(buffer, "Failed to update loan status.\n");
            } 
            else if (strcmp(new_status, "Approved") == 0) 
            {
                if (credit_to_customer(loan_id, loan_amount) == 0) 
                {
                    strcpy(buffer, "Loan approved and amount credited to customer.\n");
                } 
                else 
                {
                    strcpy(buffer, "Loan approved, but failed to credit amount.\n");
                }
            } else {
                strcpy(buffer, "Loan status updated successfully.\n");
            }
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "VIEW_CUST_TRANS") == 0)
        {
            printf("Processing VIEW_CUST_TRANS request\n");
            view_customer_transactions(sock);
        }
        else if (strcmp(buffer, "CHANGE_PWD") == 0) 
        {
            char new_pwd[50];
            read(sock, new_pwd, sizeof(new_pwd));
            new_pwd[sizeof(new_pwd) - 1] = '\0';
            size_t len = strlen(new_pwd);
            if (len > 0 && new_pwd[len - 1] == '\n') 
            {
                new_pwd[len - 1] = '\0';
            }
            int status = change_emp_password(id, new_pwd);
            if (status == 0)
                snprintf(buffer, sizeof(buffer), "Password changed\n");
            else
                snprintf(buffer, sizeof(buffer), "Failed to change password try again...\n");
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "LOGOUT") == 0)
        {
            handle_logout(sock);
            break;
        }
    }
}
