#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "bank.h"

#define PASS_LEN 20

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){ perror("Socket"); exit(1); }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Connect"); exit(1);
    }
    printf("Connected to server at 127.0.0.1:8080\n");

    int choice, logged_in = 0;
    char buf[256];

    while(1){
        printf("\n===== Customer Menu =====\n");
        printf("1.Login\n2.View Balance\n3.Deposit\n4.Withdraw\n5.Transfer\n6.Transaction History\n");
        printf("7.Change Password\n8.Add Feedback\n9.Apply for Loan\n10.Logout\n11.Exit\n");
        printf("Enter choice: "); scanf("%d",&choice);

        if(choice == 1){
            int id; char pass[PASS_LEN];
            printf("Enter ID: "); scanf("%d",&id);
            printf("Password: "); scanf("%s",pass);

            write(sock, "LOGIN", 5);
            write(sock, &id, sizeof(int));
            write(sock, pass, sizeof(pass));

            read(sock, buf, sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf);
            if(strstr(buf,"success")) logged_in = 1;
        }
        else if(choice == 2){
            write(sock,"VIEW",4);
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf);
        }
        else if(choice == 3){
            if(!logged_in){printf("Login first\n"); continue;}
            double amt; printf("Deposit amount: "); scanf("%lf",&amt);
            write(sock,"DEPOSIT",7); write(sock,&amt,sizeof(double));
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf);
        }
        else if(choice == 4){
            if(!logged_in){printf("Login first\n"); continue;}
            double amt; printf("Withdraw amount: "); scanf("%lf",&amt);
            write(sock,"WITHDRAW",8); write(sock,&amt,sizeof(double));
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf);
        }
        else if(choice == 5){
            if(!logged_in){printf("Login first\n"); continue;}
            int to_id; double amt;
            printf("Recipient ID: "); scanf("%d",&to_id);
            printf("Amount: "); scanf("%lf",&amt);
            write(sock,"TRANSFER",8); write(sock,&to_id,sizeof(int)); write(sock,&amt,sizeof(double));
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf);
        }
        else if(choice == 6){
            if(!logged_in){printf("Login first\n"); continue;}
            write(sock,"HISTORY",7); read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf);
        }
        else if(choice == 7){
            if(!logged_in){printf("Login first\n"); continue;}
            char newpass[PASS_LEN]; printf("New password: "); scanf("%s",newpass);
            write(sock,"CHPASS",6); write(sock,newpass,sizeof(newpass));
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0'; printf("%s\n",buf);
        }
        else if(choice == 8){
            if(!logged_in){printf("Login first\n"); continue;}
            char fb[200]; printf("Feedback: "); scanf(" %[^\n]s",fb);
            write(sock,"FEEDBACK",8); write(sock,fb,sizeof(fb));
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0'; printf("%s\n",buf);
        }
        else if(choice == 9){
            if(!logged_in){printf("Login first\n"); continue;}
            double amt; printf("Loan amount: "); scanf("%lf",&amt);
            write(sock,"APPLYLOAN",9); write(sock,&amt,sizeof(double));
            read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0'; printf("%s\n",buf);
        }
        else if(choice == 10){
            write(sock,"LOGOUT",6); read(sock,buf,sizeof(buf)-1); buf[strlen(buf)]='\0';
            printf("%s\n",buf); logged_in=0;
        }
        else if(choice == 11){
            write(sock,"EXIT",4); break;
        }
        else printf("Invalid choice!\n");
    }

    close(sock);
    return 0;
}
