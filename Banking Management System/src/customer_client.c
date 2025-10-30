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

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_customer_requests(int sock, const char *user_id)
{
    int id = atoi(user_id);
    while (1)
    {
        char buffer[BUFFER_SIZE];
        int bytes_read = read(sock, buffer, BUFFER_SIZE);
        buffer[bytes_read] = '\0';

        if (strcmp(buffer, "VIEW_BALANCE") == 0)
        {
            float balance = get_balance(id);
            snprintf(buffer, sizeof(buffer), "%.2f", balance);
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "DEPOSIT_MONEY") == 0)
        {
            float amount;
            int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%f", &amount);

            int status = deposit(id, amount);
            if(status == 0)
                snprintf(buffer, sizeof(buffer), "Money deposited successfuly");
            else
                snprintf(buffer, sizeof(buffer), "Failed to deposit money");

            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "WITHDRAW_MONEY") == 0)
        {
            float amount;
            int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%f", &amount);
            int status = withdraw(id, amount);
            if(status == 0)
                snprintf(buffer, sizeof(buffer), "Money withdrawn successfuly");
            else
                snprintf(buffer, sizeof(buffer), "Failed to withdraw money");

            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "TRANSFER_FUNDS") == 0)
        {
            int to_user;
            float amount;
            int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%d", &to_user);
            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%f", &amount);
            int status = transfer(id, to_user, amount);
            if (status == -1)
                snprintf(buffer, sizeof(buffer), "Failed to transfer funds\n");
            else
                snprintf(buffer, sizeof(buffer), "Funds Transfer Successful\n");
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "APPLY_LOAN") == 0)
        {
            float amount;
            int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0'; 
            sscanf(buffer, "%f", &amount);
            int status = apply_loan(id, amount); 
            if(status == 0)
                snprintf(buffer, sizeof(buffer), "Applied for loan successfuly");
            else
                snprintf(buffer, sizeof(buffer), "Failed to apply for loan");

            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "CHANGE_PWD") == 0) 
        {
            char new_pwd[50];
            read(sock, new_pwd, sizeof(new_pwd));
            new_pwd[sizeof(new_pwd) - 1] = '\0';
            size_t len = strlen(new_pwd);
            if (len > 0 && new_pwd[len - 1] == '\n') {
                new_pwd[len - 1] = '\0';
            }
            int status = change_cust_password(id, new_pwd);
            if (status == 0)
                snprintf(buffer, sizeof(buffer), "Password changed\n");
            else
                snprintf(buffer, sizeof(buffer), "Failed to change password try again...\n");
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "FEEDBACK") == 0) 
        {
            char feedback[500];
            read(sock, feedback, sizeof(feedback));
            feedback[sizeof(feedback) - 1] = '\0';
            size_t len = strlen(feedback);
            if (len > 0 && feedback[len - 1] == '\n') 
            {
                feedback[len - 1] = '\0';
            }
            int status = add_feedback(id, feedback);
            if(status == 0)
                snprintf(buffer, sizeof(buffer), "Successfully Submitted");
            else
                snprintf(buffer, sizeof(buffer), "Failed to submit feedback");
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "VIEW_TRANSACTION") == 0) 
        {
            int status = view_transaction_history(sock, id);
            if (status == -1)
            {   
                snprintf(buffer, sizeof(buffer), "Unable to show transaction history due to unknown error");
                write(sock, buffer, strlen(buffer));
            }
            write(sock, "END", strlen("END"));
        }
        else if (strcmp(buffer, "LOGOUT") == 0)
        {
            handle_logout(sock);
            break;
        }
    }
}

