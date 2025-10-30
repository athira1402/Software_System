#ifndef CLIENT_IO_H
#define CLIENT_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define CUSTOMER_DB "data/customers.dat"
#define EMPLOYEE_DB "data/employees.dat"
#define MANAGER_DB "data/manager.dat"
#define ADMIN_DB "data/admin.dat"
#define LOAN_DB "data/loan.dat"
#define TRANSACTION_DB "data/transaction.data"
#define FEEDBACK_DB "data/feedback.data"
#define TEMP_CUST_DB "data/temp_customer.data"
#define TEMP_EMP_DB "data/temp_employee.data"

#define PORT 8080
#define BUFFER_SIZE 1024


void send_request(int sock, const char *request);

void receive_response(int sock, char *buffer, size_t buffer_size);

void receive_tresponse(int sock);

#endif // CLIENT_IO_H
