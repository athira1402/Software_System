#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>  
#include <sys/wait.h> 
#include "../include/bank.h"

#define PORT 8080

pid_t current_server_pid = -1;

void stop_current_server() {
    if (current_server_pid > 0) {
        printf("Stopping old server (PID %d)...\n", current_server_pid);
        kill(current_server_pid, SIGTERM);  // graceful stop
        waitpid(current_server_pid, NULL, 0); // clean up zombie
        current_server_pid = -1;
    }
}


void start_customer_server() {
    pid_t pid = fork();
    if (pid == 0) {
        execl("./server", "server", "1", NULL); // 1 = Customer
        perror("Failed to start bank_server for customer");
        exit(1);
    } else if (pid > 0) {
        current_server_pid = pid; 
        sleep(1); // give server a sec
        start_customer_client();
    }
}

void start_employee_server() {
    pid_t pid = fork();
    if (pid == 0) {
        execl("./server", "server", "2", NULL); // 1 = Customer
        perror("Failed to start bank_server for employee");
        exit(1);
    } else if (pid > 0) {
        current_server_pid = pid; 
        sleep(1); // give server a sec
        start_employee_client();
    }
}

void start_manager_server() {
    printf("Manager module not implemented yet.\n");
}

void start_admin_server() {
    printf("Admin module not implemented yet.\n");
}


int main() {
    

    while (1) {
        
        printf("\n=======================================\n");
        printf("      Welcome to the Online Bank\n");
        printf("=======================================\n");

        printf("\nPlease select your role to continue:\n");
        printf("1. Customer\n");
        printf("2. Employee\n");
        printf("3. Manager\n");
        printf("4. Administrator\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        int role_choice;
        if (scanf("%d", &role_choice) != 1) {
            printf("\nInvalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        while (getchar() != '\n');
        
        switch (role_choice) {
            case 1:
                printf("Loading customer services...\n\n");
                stop_current_server();
                start_customer_server();
                break;

            case 2:
                printf("Loading employee services...\n\n");
                stop_current_server();
                start_employee_server();
                break;

            case 3:
                printf("Loading manager services...\n\n");
                stop_current_server();
                start_manager_server();
                break;

            case 4:
                printf("Opening admin control panel...\n\n");
                stop_current_server();
                start_admin_server();
                break;

            case 5:
                printf("\nThank you for using the Online Bank. Goodbye!\n\n");
                stop_current_server();
                exit(0);

            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}
