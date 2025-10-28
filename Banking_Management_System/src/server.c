#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "../include/bank.h"   // include your definitions
#include "../include/session.h"

#define PORT 8080

void handle_client(int client_sock, int dbfd) {
    char buffer[100], password[PASS_LEN], id_str[20];
    int n;
    struct Customer cust;
    int recno=-1;
    //int logged_in=0;

    while (1) {
        bzero(buffer, sizeof(buffer));
        n = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) break;   // client disconnected
        buffer[n] = '\0';

        if (strncmp(buffer, "LOGIN", 5) == 0) {
            // receive ID
            bzero(id_str, sizeof(id_str));
            n = recv(client_sock, id_str, sizeof(id_str) - 1, 0);
            if (n <= 0) break;
            id_str[n] = '\0';
            id_str[strcspn(id_str, "\n")] = '\0';
            int id = atoi(id_str);

            // receive password
            bzero(password, sizeof(password));
            n = recv(client_sock, password, sizeof(password) - 1, 0);
            if (n <= 0) break;
            password[n] = '\0';
            password[strcspn(password, "\n")] = '\0';
            
            recno=authenticate(dbfd, id, password, &cust);
            printf("recno=%d\n",recno);
            if (recno >= 0) {  // successful authentication
                if (is_logged_in(cust.id,CUSTOMER_SESSION_FILE)) {
                    recno=-1;
                    send(client_sock, "User already logged in elsewhere\n", 33, 0);
                } else {
                    add_session(cust.id,CUSTOMER_SESSION_FILE); // mark user as logged in
                    char msg[200];
                    //logged_in=1;
                    snprintf(msg, sizeof(msg), "Login successful! Welcome %s.\n", cust.username);
                    send(client_sock, msg, strlen(msg), 0);
                }
            } else {  // authentication failed
                send(client_sock, "Invalid ID or password\n", 24, 0);
            }
        }
        else if (strncmp(buffer, "VIEW_BAL", 8) == 0) {
            if (recno < 0) {
                send(client_sock, "You are not logged in!\n", 24, 0);
            } else {
                char msg[100];
                snprintf(msg, sizeof(msg), "Your account balance: %.2f\n", cust.balance);
                send(client_sock, msg, strlen(msg), 0);
            }
        }
        else if (strncmp(buffer, "DEPOSIT", 7) == 0) {
            char amt_str[50];
            n = recv(client_sock, amt_str, sizeof(amt_str)-1, 0);
            if (n > 0) {
                amt_str[n] = '\0';
                double amount = atof(amt_str);
                if (recno < 0) {
                    send(client_sock, "You are not logged in!\n", 24, 0);
                } else if (amount <= 0) {
                    send(client_sock, "Invalid amount!\n", 16, 0);
                } else {
                    deposit(dbfd, recno, &cust, amount);
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Deposited %.2f successfully.\n", amount);
                    send(client_sock, msg, strlen(msg), 0);
                }
            }
        }
        else if (strncmp(buffer, "WITHDRAW", 8) == 0) {
            char amt_str[50];
            n = recv(client_sock, amt_str, sizeof(amt_str)-1, 0);
            if (n > 0) {
                amt_str[n] = '\0';
                double amount = atof(amt_str);
                if (recno < 0) {
                    send(client_sock, "You are not logged in!\n", 24, 0);
                } else if (amount <= 0 || amount > cust.balance) {
                    send(client_sock, "Invalid amount or insufficient balance!\n", 40, 0);
                } else {
                    withdraw(dbfd, recno, &cust, amount);
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Withdrew %.2f successfully.\n", amount);
                    send(client_sock, msg, strlen(msg), 0);
                }
            }
        }
        else if (strncmp(buffer, "TRANSFER", 8) == 0) {
            if (recno < 0) {
                send(client_sock, "You must login first!\n", 24, 0);
                continue;
            }

            // Receive recipient ID
            char line[100];
            int n = recv(client_sock, line, sizeof(line) - 1, 0);
            if (n <= 0) break;
            line[n] = '\0';

            int to_id;
            double amount;
            if (sscanf(line, "%d %lf", &to_id, &amount) != 2) {
                send(client_sock, "Invalid input format! Use: <ID> <Amount>\n", 41, 0);
                continue;
            }
            
            int res = transfer_funds(dbfd, recno, &cust, to_id, amount);
            if (res == 0) {
                send(client_sock, "Transfer successful!\n", 21, 0);
            } else {
                send(client_sock, "Transfer failed! Check balance or recipient ID.\n", 48, 0);
            }
        }
        else if (strncmp(buffer, "APPLY_LOAN", 10) == 0) {
            if (recno < 0) {
                send(client_sock, "You must login first!\n", 23, 0);
                continue;
            }

            // Receive loan amount
            char amt_str[50];
            n = recv(client_sock, amt_str, sizeof(amt_str) - 1, 0);
            if (n <= 0) break;
            amt_str[n] = '\0';
            double amount = atof(amt_str);

            int res = apply_loan("data/loans.dat", cust.id, amount);
            if (res == 0) {
                char msg[100];
                snprintf(msg, sizeof(msg),
                        "Loan application submitted for %.2f. Status: Pending\n",
                        amount);
                send(client_sock, msg, strlen(msg), 0);
            } else {
                send(client_sock, "Loan application failed!\n", 26, 0);
            }
        }
        else if (strncmp(buffer, "CHANGE_PASS", 11) == 0) {
            if (recno < 0) {
                send(client_sock, "You are not logged in!\n", 24, 0);
            } else {
                char new_pass[PASS_LEN];
                bzero(new_pass, sizeof(new_pass));
                n = recv(client_sock, new_pass, sizeof(new_pass) - 1, 0);
                if (n <= 0) return;
                new_pass[n] = '\0';
                new_pass[strcspn(new_pass, "\n")] = '\0';

                change_password(dbfd, recno, &cust, new_pass);
                send(client_sock, "Password changed successfully!\n", 31, 0);
            }
        }

        else if (strncmp(buffer, "FEEDBACK", 8) == 0) {
            if (recno < 0) {
                send(client_sock, "You are not logged in!\n", 24, 0);
            } else {
                char feedback[200];
                bzero(feedback, sizeof(feedback));
                n = recv(client_sock, feedback, sizeof(feedback) - 1, 0);
                if (n <= 0) return;
                feedback[n] = '\0';
                feedback[strcspn(feedback, "\n")] = '\0';

                add_feedback("feedback.dat", cust.id, feedback);
                send(client_sock, "Feedback submitted successfully!\n", 33, 0);
            }
        }

        else if (strncmp(buffer, "VIEW_TRANS", 10) == 0) {
            if (recno < 0) {
                const char *msg = "You are not logged in!\n<END_OF_TRANSACTIONS>";
                send(client_sock, msg, strlen(msg), 0); 
            } else {
                view_transactions(client_sock, "transactions.dat", cust.id);

                // send marker to indicate end of transmission
                send(client_sock, "<END_OF_TRANSACTIONS>", 22, 0);
            }
        }

        else if (strncmp(buffer, "LOGOUT", 6) == 0) {
            if (recno>=0){
                //logged_in=0;
                recno=-1;
                remove_session(cust.id,CUSTOMER_SESSION_FILE); // remove session after logout/disconnect
                send(client_sock, "Logged out\n", 11, 0);
            }
            else{
                send(client_sock, "You are not logged in!\n", 24, 0);
            }
            
        }
    }

    close(client_sock);
    if (recno >= 0) {
        remove_session(cust.id,CUSTOMER_SESSION_FILE);
    }
}

void handle_employee(int client_sock, int dbfd) {
    char buffer[100], password[PASS_LEN], id_str[20];
    int n;
    struct Customer cust;
    int recno=-1;
    //int logged_in=0;

    while (1) {
        bzero(buffer, sizeof(buffer));
        n = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) break;   // client disconnected
        buffer[n] = '\0';

        if (strncmp(buffer, "LOGIN", 5) == 0) {
            // receive ID
            bzero(id_str, sizeof(id_str));
            n = recv(client_sock, id_str, sizeof(id_str) - 1, 0);
            if (n <= 0) break;
            id_str[n] = '\0';
            id_str[strcspn(id_str, "\n")] = '\0';
            int id = atoi(id_str);

            // receive password
            bzero(password, sizeof(password));
            n = recv(client_sock, password, sizeof(password) - 1, 0);
            if (n <= 0) break;
            password[n] = '\0';
            password[strcspn(password, "\n")] = '\0';
            
            recno=authenticate(dbfd, id, password, &cust);
            printf("recno=%d\n",recno);
            if (recno >= 0) {  // successful authentication
                if (is_logged_in(cust.id,EMPLOYEE_SESSION_FILE)) {
                    recno=-1;
                    send(client_sock, "Employee already logged in elsewhere\n", 33, 0);
                } else {
                    add_session(cust.id,EMPLOYEE_SESSION_FILE); // mark user as logged in
                    char msg[200];
                    //logged_in=1;
                    snprintf(msg, sizeof(msg), "Login successful! Welcome %s.\n", cust.username);
                    send(client_sock, msg, strlen(msg), 0);
                }
            } else {  // authentication failed
                send(client_sock, "Invalid ID or password\n", 24, 0);
            }
        }
        else if (strncmp(buffer, "ADD_CUST", 8) == 0) {
            if (recno < 0) {
                send(client_sock, "You are not logged in!\n", 24, 0);
            } else {
                char data[200];
                char name[NAME_LEN], pass[PASS_LEN];
                double balance;

                n = recv(client_sock, data, sizeof(data) - 1, 0);
                if (n <= 0) break;
                data[n] = '\0';

                // Parse the received data
                char *name_token = strtok(data, "|");
                char *pass_token = strtok(NULL, "|");
                char *balance_token = strtok(NULL, "|");

                if (name_token && pass_token && balance_token) {
                    // Copy tokens safely
                    strncpy(name, name_token, NAME_LEN - 1);
                    name[NAME_LEN - 1] = '\0';
                    strncpy(pass, pass_token, PASS_LEN - 1);
                    pass[PASS_LEN - 1] = '\0';
                    balance = atof(balance_token);

                    int cust_dbfd = open("data/customers.dat", O_RDWR | O_CREAT, 0666);
                    if (cust_dbfd < 0) {
                        send(client_sock, "Server Error: Could not open customer database.\n", 48, 0);
                        continue;
                    }
                    
                    // Call the function to create the struct Customer and write it to customers.dat
                    int new_id = add_customer(cust_dbfd, name, pass, balance);
                    close(cust_dbfd); // Close after operation

                    if (new_id > 0) {
                        char msg[100];
                        snprintf(msg, sizeof(msg), "Customer added successfully! New Customer ID: %d\n", new_id);
                        send(client_sock, msg, strlen(msg), 0);
                    } else {
                        send(client_sock, "Failed to add customer due to a database error.\n", 48, 0);
                    }
                }
            }
        }
        else if (strncmp(buffer, "DEPOSIT", 7) == 0) {
            char amt_str[50];
            n = recv(client_sock, amt_str, sizeof(amt_str)-1, 0);
            if (n > 0) {
                amt_str[n] = '\0';
                double amount = atof(amt_str);
                if (recno < 0) {
                    send(client_sock, "You are not logged in!\n", 24, 0);
                } else if (amount <= 0) {
                    send(client_sock, "Invalid amount!\n", 16, 0);
                } else {
                    deposit(dbfd, recno, &cust, amount);
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Deposited %.2f successfully.\n", amount);
                    send(client_sock, msg, strlen(msg), 0);
                }
            }
        }
        else if (strncmp(buffer, "WITHDRAW", 8) == 0) {
            char amt_str[50];
            n = recv(client_sock, amt_str, sizeof(amt_str)-1, 0);
            if (n > 0) {
                amt_str[n] = '\0';
                double amount = atof(amt_str);
                if (recno < 0) {
                    send(client_sock, "You are not logged in!\n", 24, 0);
                } else if (amount <= 0 || amount > cust.balance) {
                    send(client_sock, "Invalid amount or insufficient balance!\n", 40, 0);
                } else {
                    withdraw(dbfd, recno, &cust, amount);
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Withdrew %.2f successfully.\n", amount);
                    send(client_sock, msg, strlen(msg), 0);
                }
            }
        }
        else if (strncmp(buffer, "TRANSFER", 8) == 0) {
            if (recno < 0) {
                send(client_sock, "You must login first!\n", 24, 0);
                continue;
            }

            // Receive recipient ID
            char line[100];
            int n = recv(client_sock, line, sizeof(line) - 1, 0);
            if (n <= 0) break;
            line[n] = '\0';

            int to_id;
            double amount;
            if (sscanf(line, "%d %lf", &to_id, &amount) != 2) {
                send(client_sock, "Invalid input format! Use: <ID> <Amount>\n", 41, 0);
                continue;
            }
            
            int res = transfer_funds(dbfd, recno, &cust, to_id, amount);
            if (res == 0) {
                send(client_sock, "Transfer successful!\n", 21, 0);
            } else {
                send(client_sock, "Transfer failed! Check balance or recipient ID.\n", 48, 0);
            }
        }
        else if (strncmp(buffer, "APPLY_LOAN", 10) == 0) {
            if (recno < 0) {
                send(client_sock, "You must login first!\n", 23, 0);
                continue;
            }

            // Receive loan amount
            char amt_str[50];
            n = recv(client_sock, amt_str, sizeof(amt_str) - 1, 0);
            if (n <= 0) break;
            amt_str[n] = '\0';
            double amount = atof(amt_str);

            int res = apply_loan("data/loans.dat", cust.id, amount);
            if (res == 0) {
                char msg[100];
                snprintf(msg, sizeof(msg),
                        "Loan application submitted for %.2f. Status: Pending\n",
                        amount);
                send(client_sock, msg, strlen(msg), 0);
            } else {
                send(client_sock, "Loan application failed!\n", 26, 0);
            }
        }
        else if (strncmp(buffer, "CHANGE_PASS", 11) == 0) {
            if (recno < 0) {
                send(client_sock, "You are not logged in!\n", 24, 0);
            } else {
                char new_pass[PASS_LEN];
                bzero(new_pass, sizeof(new_pass));
                n = recv(client_sock, new_pass, sizeof(new_pass) - 1, 0);
                if (n <= 0) return;
                new_pass[n] = '\0';
                new_pass[strcspn(new_pass, "\n")] = '\0';

                change_password(dbfd, recno, &cust, new_pass);
                send(client_sock, "Password changed successfully!\n", 31, 0);
            }
        }

        else if (strncmp(buffer, "FEEDBACK", 8) == 0) {
            if (recno < 0) {
                send(client_sock, "You are not logged in!\n", 24, 0);
            } else {
                char feedback[200];
                bzero(feedback, sizeof(feedback));
                n = recv(client_sock, feedback, sizeof(feedback) - 1, 0);
                if (n <= 0) return;
                feedback[n] = '\0';
                feedback[strcspn(feedback, "\n")] = '\0';

                add_feedback("feedback.dat", cust.id, feedback);
                send(client_sock, "Feedback submitted successfully!\n", 33, 0);
            }
        }

        else if (strncmp(buffer, "VIEW_TRANS", 10) == 0) {
            if (recno < 0) {
                const char *msg = "You are not logged in!\n<END_OF_TRANSACTIONS>";
                send(client_sock, msg, strlen(msg), 0); 
            } else {
                view_transactions(client_sock, "transactions.dat", cust.id);

                // send marker to indicate end of transmission
                send(client_sock, "<END_OF_TRANSACTIONS>", 22, 0);
            }
        }

        else if (strncmp(buffer, "LOGOUT", 6) == 0) {
            if (recno>=0){
                //logged_in=0;
                recno=-1;
                remove_session(cust.id,CUSTOMER_SESSION_FILE); // remove session after logout/disconnect
                send(client_sock, "Logged out\n", 11, 0);
            }
            else{
                send(client_sock, "You are not logged in!\n", 24, 0);
            }
            
        }
    }

    close(client_sock);
    if (recno >= 0) {
        remove_session(cust.id,CUSTOMER_SESSION_FILE);
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    int role = atoi(argv[1]);
    const char *db_file = NULL;
    switch (role) {
        case 1:
            db_file = "data/customers.dat";
            break;
        case 2:
            db_file = "data/employees.dat";
            break;
        case 3:
            db_file = "data/managers.dat";
            break;
        case 4:
            db_file = "data/admins.dat";
            break;
        default:
            fprintf(stderr, "Invalid role! (1=Customer, 2=Employee, 3=Manager, 4=Admin)\n");
            exit(1);
    }
    printf("Starting bank server for role %d using DB file: %s\n", role, db_file);


    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_sock, 5);
    printf("Server listening on port %d...\n", PORT);

    // open customer database file
    //int dbfd = open("data/customers.dat", O_RDWR);
    int dbfd = open(db_file, O_RDWR);
    if (dbfd < 0) {
        perror("Error opening customers.dat");
        exit(1);
    }

    while (1) {
        int client_sock = accept(server_sock, NULL, NULL);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        if (fork() == 0) {
            close(server_sock);
            if (role==1) handle_client(client_sock, dbfd);
            else if (role==2) handle_employee(client_sock, dbfd);
            exit(0);
        } else {
            close(client_sock);
        }
    }

    close(server_sock);
    close(dbfd);
    return 0;
}
