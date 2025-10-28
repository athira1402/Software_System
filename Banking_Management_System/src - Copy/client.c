#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#define PASS_LEN 20
int sock;

void handle_sig(int sig) {
    (void)sig;
    if (sock > 0) close(sock);
    printf("\nClient socket closed. Exiting.\n");
    exit(0);
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("Socket"); exit(1); }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    signal(SIGINT, handle_sig);
    signal(SIGTSTP, handle_sig);
    signal(SIGTERM, my_handler);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect"); exit(1);
    }
    printf("Connected to server at 127.0.0.1:8080\n");

    int choice, logged_in = 0;
    char buf[256];

    while (1) {
        printf("\n===== Customer Menu =====\n");
        printf("1.Login\n2.View Balance\n3.Deposit\n4.Withdraw\n5.Transfer\n6.Transaction History\n");
        printf("7.Change Password\n8.Add Feedback\n9.Apply for Loan\n10.Logout\n11.Exit\n");
        printf("Enter choice: "); scanf("%d", &choice);
        getchar(); // consume newline

        memset(buf, 0, sizeof(buf));

        if (choice == 1) {
            int id; char pass[PASS_LEN] = {0};
            printf("Enter ID: "); scanf("%d", &id);
            printf("Password: "); scanf("%s", pass);

            write(sock, "LOGIN", 5);
	    printf("Sent command: LOGIN\n");
            write(sock, &id, sizeof(int));
            printf("Sent command id: LOGIN\n");
            write(sock, pass, sizeof(pass));
            printf("Sent command pass: LOGIN\n");

            int n = read(sock, buf, sizeof(buf)-1);
            printf("Received %d bytes from server\n", n);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
            if (strstr(buf, "success")) logged_in = 1;
        }
        else if (choice == 2) {
            write(sock, "VIEW", 4);
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 3 && logged_in) {
            double amt;
            printf("Deposit amount: "); scanf("%lf", &amt);
            write(sock, "DEPOSIT", 7);
            write(sock, &amt, sizeof(double));
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 4 && logged_in) {
            double amt;
            printf("Withdraw amount: "); scanf("%lf", &amt);
            write(sock, "WITHDRAW", 8);
            write(sock, &amt, sizeof(double));
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 5 && logged_in) {
            int to_id; double amt;
            printf("Recipient ID: "); scanf("%d", &to_id);
            printf("Amount: "); scanf("%lf", &amt);
            write(sock, "TRANSFER", 8);
            write(sock, &to_id, sizeof(int));
            write(sock, &amt, sizeof(double));
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 6 && logged_in) {
            write(sock, "HISTORY", 7);
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 7 && logged_in) {
            char newpass[PASS_LEN] = {0};
            printf("New password: "); scanf("%s", newpass);
            write(sock, "CHPASS", 6);
            write(sock, newpass, sizeof(newpass));
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 8 && logged_in) {
            char fb[200] = {0};
            printf("Feedback: "); scanf(" %[^\n]", fb);
            write(sock, "FEEDBACK", 8);
            write(sock, fb, sizeof(fb));
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 9 && logged_in) {
            double amt;
            printf("Loan amount: "); scanf("%lf", &amt);
            write(sock, "APPLYLOAN", 9);
            write(sock, &amt, sizeof(double));
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
        }


        else if (choice == 10) {
            write(sock, "LOGOUT", 6);
            int n = read(sock, buf, sizeof(buf)-1);
            if (n > 0) { buf[n] = '\0'; printf("%s\n", buf); }
            logged_in = 0;
        }


        else if (choice == 11) {
            write(sock, "EXIT", 4);
            break;
        }


        else {
            printf("Invalid choice or not logged in.\n");
        }
        
    }

    close(sock);
    return 0;
}
