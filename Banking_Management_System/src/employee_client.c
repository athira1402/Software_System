#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 8080

void start_employee_client() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    char choice[10], buffer[100], id[50], password[50];
    
    int n;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    while (1) {
        printf("\n--- MENU ---\n1. Login\n2. Add New Customer\n3. Modify Customer Details\n4. Process Loan applications\n5. Approve/Reject Loans\n6. View Assigned Loan Applications\n7. View Customer Transactions\n8. Change Password\n9. Logout \n10. Exit\n\nChoose: ");
        fgets(choice, sizeof(choice), stdin);
        int opt = atoi(choice);

        if (opt == 1) {
            //send(sock, "CUSTOMER_LOGIN", 14, 0);
            send(sock, "LOGIN", 5, 0);

            printf("Enter Employee Login ID: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            send(sock, id, strlen(id), 0);

            printf("Enter Password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = 0;
            send(sock, password, strlen(password), 0);

            bzero(buffer, sizeof(buffer));
            n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Server: %s\n", buffer);
            }
        }
        else if (opt == 2){
            send(sock, "ADD_CUST", 8, 0); // new command
           char name[50], new_pass[50], balance_str[50], data_to_send[200];

            printf("Enter new customer name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Enter initial password: ");
            fgets(new_pass, sizeof(new_pass), stdin);
            new_pass[strcspn(new_pass, "\n")] = 0;

            printf("Enter initial balance (e.g., 500.00): ");
            fgets(balance_str, sizeof(balance_str), stdin);
            balance_str[strcspn(balance_str, "\n")] = 0;
            
            // Format data as a single string: "NAME|PASSWORD|BALANCE"
            snprintf(data_to_send, sizeof(data_to_send), "%s|%s|%s", name, new_pass, balance_str);

            send(sock, "ADD_CUST", 8, 0);
            send(sock, data_to_send, strlen(data_to_send), 0);

            // Receive confirmation message (new ID or error)
            bzero(buffer, sizeof(buffer));
            n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Server: %s\n", buffer);
            }

        }
        else if (opt == 3) { // Deposit
        char amt_str[50];
        //double amt;

        printf("Enter amount to deposit: ");
        fgets(amt_str, sizeof(amt_str), stdin);
        //amt = atof(amt_str);

        send(sock, "DEPOSIT", 7, 0);
        send(sock, amt_str, strlen(amt_str), 0);

        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (n>0) { buffer[n]='\0'; printf("Server: %s\n", buffer); }
    }
    else if (opt == 4) { // Withdraw
        char amt_str[50];
        //double amt;

        printf("Enter amount to withdraw: ");
        fgets(amt_str, sizeof(amt_str), stdin);
        //amt = atof(amt_str);

        send(sock, "WITHDRAW", 8, 0);
        send(sock, amt_str, strlen(amt_str), 0);

        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (n>0) { buffer[n]='\0'; printf("Server: %s\n", buffer); }
    }
    else if (opt == 5) {  // Transfer Funds
        char to_id[20], amt_str[20], msg[100];

        printf("Enter recipient ID: ");
        fgets(to_id, sizeof(to_id), stdin);
        to_id[strcspn(to_id, "\n")] = 0; // remove newline

        printf("Enter amount: ");
        fgets(amt_str, sizeof(amt_str), stdin);
        amt_str[strcspn(amt_str, "\n")] = 0; // remove newline

        snprintf(msg, sizeof(msg), "%s %s", to_id, amt_str);

        send(sock, "TRANSFER", 8, 0);
        send(sock, msg, strlen(msg), 0);
        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server: %s\n", buffer);
        }
    }
    else if (opt == 6) {  // Apply Loan
        char amt_str[50];
        printf("Enter loan amount: ");
        fgets(amt_str, sizeof(amt_str), stdin);

        send(sock, "APPLY_LOAN", 10, 0);
        send(sock, amt_str, strlen(amt_str), 0);

        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server: %s\n", buffer);
        }
    }
    if (opt == 7) {  // Change Password
        char new_pass[50];
        send(sock, "CHANGE_PASS", 11, 0);
        printf("Enter New Password: ");
        fgets(new_pass, sizeof(new_pass), stdin);
        send(sock, new_pass, strlen(new_pass), 0);

        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server: %s\n", buffer);
        }
    }

    else if (opt == 8) {  // Feedback
        char feedback[200];
        send(sock, "FEEDBACK", 8, 0);
        printf("Enter your feedback: ");
        fgets(feedback, sizeof(feedback), stdin);
        send(sock, feedback, strlen(feedback), 0);

        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server: %s\n", buffer);
        }
    }

    else if (opt == 9) {  // View Transactions
        send(sock, "VIEW_TRANS", 10, 0);
        char fullbuf[4096] = {0};
        //int total_len = 0;

        while (1) {
            n = recv(sock, buffer, sizeof(buffer) - 1, 0);  // blocking read
            if (n <= 0) break;  // connection closed or error
            buffer[n] = '\0';

            strncat(fullbuf, buffer, sizeof(fullbuf) - strlen(fullbuf) - 1);

            if (strstr(fullbuf, "<END_OF_TRANSACTIONS>"))
                break;
            }
            // // check if end marker exists anywhere
            // char *pos = strstr(buffer, "<END_OF_TRANSACTIONS>");
            // if (pos != NULL) {
            //     *pos = '\0';  // terminate string at marker
            //     if (strlen(buffer) > 0)
            //         printf("%s", buffer);  // print anything before marker
            //     break;
            // }
            char *pos = strstr(fullbuf, "<END_OF_TRANSACTIONS>");
            if (pos) *pos = '\0';
            printf("%s\n", fullbuf);

            //printf("%s", buffer);  // print chunk
        //}
        //printf("\n");
    }

    else if (opt == 10) {
        send(sock, "LOGOUT", 6, 0);
        bzero(buffer, sizeof(buffer));
        n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server: %s\n", buffer);
        }
    }
    else if (opt == 11) {  // Exit
    printf("Exiting...\n");
    close(sock);
    exit(0);
    }
    }

    close(sock);
}

