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

void handle_manager_requests(int sock, const char *user_id)
{
    int id = atoi(user_id);
    while (1)
    {
        char buffer[BUFFER_SIZE];
        int bytes_read = read(sock, buffer, BUFFER_SIZE);
        buffer[bytes_read] = '\0';

        if (strcmp(buffer, "ACT/DEACT CUST ACC") == 0)
        {
            int custid;
            char action[20];

            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%d", &custid);

            bytes_read = read(sock, action, sizeof(action) - 1);
            action[bytes_read] = '\0';
            action[strcspn(action, "\n")] = '\0';

            if (update_customer_status(custid, action)) 
            {
                sprintf(buffer, "Customer %d %s successfully.\n", custid, action);
            } 
            else 
            {
                sprintf(buffer, "Failed to %s customer %d.\n", action, custid);
            }
            write(sock, buffer, strlen(buffer));
        }
        else if(strcmp(buffer, "ASSIGN LOAN") == 0)
        {
            int loanID, empID;

            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%d", &loanID);

            bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            if (bytes_read <= 0) 
            {
                perror("Error reading Employee ID");
                close(sock);
                return;
            }
            buffer[bytes_read] = '\0';
            empID = atoi(buffer);

            if (assign_loan_to_employee(empID, loanID))
            {
                sprintf(buffer, "Loan %d assigned to Employee %d successfully.\n", loanID, empID);
            } 
            else 
            {
                sprintf(buffer, "Failed to assign loan %d to Employee %d.\n", loanID, empID);
            }
            write(sock, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "VIEW_FEEDBACK") == 0) 
        {
            FILE *file = fopen("./data/feedback.data", "rb");
            if (!file) 
            {
                perror("Error opening feedback file");
                strcpy(buffer, "No feedback available.\n");
                write(sock, buffer, strlen(buffer));
                return;
            }

            struct feedback fb;
            char feedbacks[BUFFER_SIZE * 10] = ""; 
            int count = 0;

            while (fread(&fb, sizeof(struct feedback), 1, file)) 
            {
                char temp[BUFFER_SIZE];
                snprintf(temp, sizeof(temp), "Feedback ID: %d\nCustomer ID: %d\nMessage: %s\nStatus: %s\n\n", fb.feedbackID, fb.customerID, fb.message, fb.status);
                strcat(feedbacks, temp);
                count++;
            }
            fclose(file);

            if (count == 0) 
            {
                strcpy(feedbacks, "No feedback available.\n");
            }

            write(sock, feedbacks, strlen(feedbacks)); 

            int feedback_id;
            int bytes_read = read(sock, buffer, BUFFER_SIZE);
            if (bytes_read <= 0) 
            {
                perror("Error reading feedback ID");
                return;
            }
            buffer[bytes_read] = '\0';
            sscanf(buffer, "%d", &feedback_id);

            FILE *temp_file = fopen("./data/temp_feedback.data", "wb");
            file = fopen("./data/feedback.data", "rb");

            if (!temp_file || !file) 
            {
                perror("Error processing feedback update");
                return;
            }

            int updated = 0;
            while (fread(&fb, sizeof(struct feedback), 1, file)) 
            {
                if (fb.feedbackID == feedback_id) 
                {
                    strcpy(fb.status, "Reviewed"); 
                    updated = 1;
                }
                fwrite(&fb, sizeof(struct feedback), 1, temp_file);
            }
            fclose(file);
            fclose(temp_file);

            remove("./data/feedback.data");
            rename("./data/temp_feedback.data", "./data/feedback.data");

            if (updated) 
            {
                strcpy(buffer, "Feedback status updated to 'Reviewed'.\n");
            } 
            else 
            {
                strcpy(buffer, "Invalid Feedback ID.\n");
            }
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
            int status = change_mng_password(id, new_pwd);
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
