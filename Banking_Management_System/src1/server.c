#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include "bank.h"

#define PORT 8080
#define MAX_CLIENTS 10

int dbfd;
int sessions[100];  // Track logged-in customers

// Prevent zombie processes
void sigchld_handler(int s) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// Handle each connected client
void handle_client(int client_sock) {
    char buf[256];
    struct Customer cust;
    int logged_in = 0;
    int recno = -1;

    while(1) {
        int n = read(client_sock, buf, sizeof(buf)-1);
	printf("Received: %s\n", buf);
	fflush(stdout);
        if(n <= 0) break;
        buf[n] = '\0';

        if(strncmp(buf, "LOGIN", 5) == 0) {
	    printf("inside login\n");
            int id; char pass[PASS_LEN];
            read(client_sock, &id, sizeof(int));
            read(client_sock, pass, sizeof(pass));
	    printf("read client id pass\n");

            if(sessions[id-1]) { write(client_sock, "Already logged in\n", 18); continue; }
            recno = authenticate(dbfd, id, pass, &cust);
            if(recno >= 0) { logged_in = 1; sessions[id-1] = 1; write(client_sock, "Login success\n", 14); }
            else write(client_sock, "Login failed\n", 13);
        }
        else if(strncmp(buf, "VIEW", 4) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            char msg[50];
            snprintf(msg, sizeof(msg), "Balance: %.2f\n", cust.balance);
            write(client_sock, msg, strlen(msg));
        }
        else if(strncmp(buf, "DEPOSIT", 7) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            double amt; read(client_sock, &amt, sizeof(double));
            deposit(dbfd, recno, &cust, amt);
            write(client_sock, "Deposit done\n", 13);
        }
        else if(strncmp(buf, "WITHDRAW", 8) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            double amt; read(client_sock, &amt, sizeof(double));
            withdraw(dbfd, recno, &cust, amt);
            write(client_sock, "Withdraw done\n", 14);
        }
        else if(strncmp(buf, "TRANSFER", 8) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            int to_id; double amt;
            read(client_sock, &to_id, sizeof(int));
            read(client_sock, &amt, sizeof(double));
            transfer_funds(dbfd, recno, &cust, to_id, amt);
            write(client_sock, "Transfer done\n", 14);
        }
        else if(strncmp(buf, "HISTORY", 7) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            view_transactions("transactions.dat", cust.id);
            write(client_sock, "Transaction history shown on server\n", 36);
        }
        else if(strncmp(buf, "CHPASS", 6) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            char newpass[PASS_LEN]; read(client_sock, newpass, sizeof(newpass));
            change_password(dbfd, recno, &cust, newpass);
            write(client_sock, "Password changed\n", 17);
        }
        else if(strncmp(buf, "FEEDBACK", 8) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            char fb[200]; read(client_sock, fb, sizeof(fb));
            add_feedback("feedback.dat", cust.id, fb);
            write(client_sock, "Feedback added\n", 15);
        }
        else if(strncmp(buf, "APPLYLOAN", 9) == 0) {
            if(!logged_in){ write(client_sock, "Login first\n", 12); continue; }
            double amt; read(client_sock, &amt, sizeof(double));
            apply_loan("loans.dat", cust.id, amt);
            write(client_sock, "Loan applied\n", 13);
        }
        else if(strncmp(buf, "LOGOUT", 6) == 0) {
            if(logged_in){ sessions[cust.id-1]=0; logged_in=0; write(client_sock,"Logged out\n",11);}
            else write(client_sock,"Not logged in\n",14);
        }
        else if(strncmp(buf, "EXIT", 4) == 0) break;
        else write(client_sock, "Unknown command\n",16);
    }

    if(logged_in) sessions[cust.id-1] = 0;
    close(client_sock);
}

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
    signal(SIGCHLD, sigchld_handler);

    // Open customer database
    dbfd = open("customers.dat", O_CREAT | O_RDWR, 0666);
    if(dbfd < 0){ perror("Database"); exit(1); }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(sockfd, MAX_CLIENTS);
    printf("Server listening on port %d\n", PORT);

    while(1) {
//        printf("in while\n");
        sin_size = sizeof(client_addr);
        new_sock = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size);
        if(new_sock < 0){ perror("Accept"); continue; }
//	printf("before fork");
        if(!fork()) { // child process
 //         printf("in fork");
            close(sockfd);
            handle_client(new_sock);
            exit(0);
        }
//      printf("after fork\n");
        close(new_sock); // parent
    }
    return 0;
}
