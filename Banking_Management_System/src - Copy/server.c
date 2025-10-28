#include "bank.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#define PORT 8080
#define MAX_CLIENTS 10


int dbfd;

//if user terminated then close connections
void my_handler(int sig) {
    printf("Received signal %d — terminating cleanly\n", sig);
    killpg(0, SIGTERM);
    close(sockfd);  // if global
    exit(0);
}

void child_handler(int sig) {
    printf("[Child %d] Received signal %d — exiting\n", getpid(), sig);
    if (new_sock > 0) close(new_sock);
    exit(0);
}



void handle_client(int sock,int *sessions) {
    char buf[256];
    struct Customer cust;
    int logged_in = 0;
    int recno = -1;

    while(1) {
        int n = read(sock, buf, sizeof(buf)-1);
        if(n <= 0) break;
        buf[n] = '\0';
        
        if(strncmp(buf, "LOGIN", 5) == 0) {
            int id; char pass[PASS_LEN];
            read(sock, &id, sizeof(int));
            read(sock, pass, sizeof(pass));

            if(id <= 0 || id > 100) {
                write(sock, "Invalid ID\n", 11);
                continue;
            }

            if(sessions[id-1]) {
                write(sock, "Already logged in\n", 18);
                continue;
            }

            recno = authenticate(dbfd, id, pass, &cust);
            if(recno >= 0) {
                logged_in = 1;
                sessions[id-1] = 1;
                write(sock, "Login success\n", 14);
            } else {
                write(sock, "Login failed\n", 13);
            }
        }
        else if(strncmp(buf, "VIEW", 4) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            char msg[64];
            snprintf(msg, sizeof(msg), "Balance: %.2f\n", cust.balance);
            write(sock, msg, strlen(msg));
        }
        else if(strncmp(buf, "DEPOSIT", 7) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            double amt;
            read(sock, &amt, sizeof(double));
            deposit(dbfd, recno, &cust, amt);
            write(sock, "Deposit done\n", 13);
        }
        else if(strncmp(buf, "WITHDRAW", 8) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            double amt;
            read(sock, &amt, sizeof(double));
            withdraw(dbfd, recno, &cust, amt);
            write(sock, "Withdraw done\n", 14);
        }
        else if(strncmp(buf, "TRANSFER", 8) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            int to_id; double amt;
            read(sock, &to_id, sizeof(int));
            read(sock, &amt, sizeof(double));
            transfer_funds(dbfd, recno, &cust, to_id, amt);
            write(sock, "Transfer done\n", 14);
        }
        else if(strncmp(buf, "HISTORY", 7) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            view_transactions("transactions.dat", cust.id);
            write(sock, "Transaction history shown\n", 27);
        }
        else if(strncmp(buf, "CHPASS", 6) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            char newpass[PASS_LEN];
            read(sock, newpass, sizeof(newpass));
            change_password(dbfd, recno, &cust, newpass);
            write(sock, "Password changed\n", 17);
        }
        else if(strncmp(buf, "FEEDBACK", 8) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            char fb[200];
            read(sock, fb, sizeof(fb));
            add_feedback("feedback.dat", cust.id, fb);
            write(sock, "Feedback added\n", 15);
        }
        else if(strncmp(buf, "APPLYLOAN", 9) == 0) {
            if(!logged_in) { write(sock, "Login first\n", 12); continue; }
            double amt;
            read(sock, &amt, sizeof(double));
            apply_loan("loans.dat", cust.id, amt);
            write(sock, "Loan applied\n", 13);
        }
        else if(strncmp(buf, "LOGOUT", 6) == 0) {
            if(logged_in) {
                sessions[cust.id-1] = 0;
                logged_in = 0;
                write(sock, "Logged out\n", 11);
            } else {
                write(sock, "Not logged in\n", 14);
            }
        }
        else if(strncmp(buf, "EXIT", 4) == 0) {
            break;
        }
        else {
            write(sock, "Unknown command\n", 16);
        }

    }
    if(logged_in) sessions[cust.id-1] = 0;
    close(sock);
}


int main() {
    setpgid(0, 0);
    int sockfd,new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    dbfd = open("data/customers.dat", O_CREAT | O_RDWR, 0666);
    if(dbfd < 0) { perror("Database"); exit(1); }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        exit(1);
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEPORT)");
        exit(1);
    }

    signal(SIGINT, my_handler);
    signal(SIGTSTP, my_handler);
    signal(SIGTERM, my_handler);
    signal(SIGCHLD, SIG_IGN);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(sockfd, MAX_CLIENTS);
    printf("Server listening on port %d\n", PORT);

    sin_size = sizeof(client_addr);

    int *sessions = mmap(NULL, sizeof(int) * 100,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sessions == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    memset(sessions, 0, sizeof(int) * 100);

    while (1) {
    new_sock = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size);

    if (new_sock < 0) {
        perror("accept");
        continue;
    }
    printf("Accepted connection\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(new_sock);
        continue;
    }
    if (pid == 0) {
        // Child
        signal(SIGTERM, child_handler);
        prctl(PR_SET_PDEATHSIG, SIGTERM);  // auto-terminate if parent dies 
        close(sockfd);
        printf("Child handling client\n");
        handle_client(new_sock,sessions);
        close(new_sock);
        exit(0);
    } else {
        // Parent
        close(new_sock);  // Important: parent must close its copy
    }
    }
    close(sockfd);
    return 0;
}





