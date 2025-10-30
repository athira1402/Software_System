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
#include "../include/client_io.h"
#include "../include/customer_ops.h"
#include "../include/transactions.h"
#include "../include/employee_ops.h"
#include "../include/manager_ops.h"
#include "../include/admin_ops.h"
#include "../include/customer_client.h"
#include "../include/employee_client.h"
#include "../include/manager_client.h"
#include "../include/admin_client.h"
#include "../include/session.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void initialize_sessions() {
    for (int i = 0; i < MAX_SESSIONS; i++) {
        active_sessions[i].is_active = 0;
        active_sessions[i].userID = 0;
        active_sessions[i].socket_descriptor = 0;
    }
}

// MODIFIED: Accepts the ID as a string
int is_user_active(const char *uid_str, const char *role) {
    int userID = atoi(uid_str); // Convert string ID to integer
    
    // Check for a conversion error (e.g., if ID is non-numeric)
    if (userID <= 0) { 
        // Log an error or handle invalid input
        return 0;
    }

    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (active_sessions[i].is_active && 
            active_sessions[i].userID == userID && // Compare integers
            strcmp(active_sessions[i].role, role) == 0) {
            return 1; // User is found and active
        }
    }
    return 0; // User is not found
}

// Returns 1 on success, 0 if MAX_SESSIONS is reached.
int add_active_session(int userID, const char *role, int sock) {
    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (!active_sessions[i].is_active) {
            // Found an empty slot
            active_sessions[i].userID = userID;
            strncpy(active_sessions[i].role, role, MAX_ROLE_LEN - 1);
            active_sessions[i].role[MAX_ROLE_LEN - 1] = '\0'; // Ensure null termination
            active_sessions[i].socket_descriptor = sock;
            active_sessions[i].is_active = 1;
            return 1;
        }
    }
    return 0; // No space for a new session
}

// Returns 1 on success, 0 if not found.
int remove_active_session(int sock) {
    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (active_sessions[i].is_active && 
            active_sessions[i].socket_descriptor == sock) {
            // Found the session associated with the socket
            active_sessions[i].is_active = 0;
            active_sessions[i].userID = 0;
            active_sessions[i].socket_descriptor = 0;
            return 1;
        }
    }
    return 0;
}

int validate_login(const char *role, const char *uid, const char *password) {
	int id = atoi(uid);
    FILE *file;
    char filename[50];
    
    if (strcmp(role, "customer") == 0) {
        strcpy(filename, CUSTOMER_DB);
    } else if (strcmp(role, "employee") == 0) {
        strcpy(filename, EMPLOYEE_DB);
    } else if (strcmp(role, "manager") == 0) {
        strcpy(filename, MANAGER_DB);
    } else if (strcmp(role, "admin") == 0) {
        strcpy(filename, ADMIN_DB);
    } else {
        return 0; 
    }

    file = fopen(filename, "rb");
    if (file == NULL) 
    {
        perror("File open error");
        return 0;
    }

    if (strcmp(role, "customer") == 0) 
    {
        struct customer c;
        while (fread(&c, sizeof(struct customer), 1, file) == 1) 
        {
            if ((c.userID == id) && strcmp(c.password, password) == 0) 
            {
                fclose(file);
                return 1;
            }
        }
    }
    else if (strcmp(role, "employee") == 0) 
    {
        struct employee emp;
        while (fread(&emp, sizeof(struct employee), 1, file) == 1) 
        {
            if ((emp.employeeID == id) && strcmp(emp.password, password) == 0) 
            {
                fclose(file);
                return 1;
            }
        }
    } 
    else if (strcmp(role, "manager") == 0) 
    {
        struct manager mgr;
        while (fread(&mgr, sizeof(struct manager), 1, file) == 1) 
        {
            if ((mgr.managerID == id) && strcmp(mgr.password, password) == 0) 
            {
                fclose(file);
                return 1; 
            }
        }
    } 
    else if (strcmp(role, "admin") == 0) 
    {
        struct admin admin;
        while (fread(&admin, sizeof(struct admin), 1, file) == 1) 
        {
            if ((admin.adminID == id) && strcmp(admin.password, password) == 0) 
            {
                fclose(file);
                return 1; 
            }
        }
    }

    fclose(file);
    return 0;
}


void *handle_client(void *client_socket)
{
    // The following functions are assumed to be defined globally on the server:
    // int add_active_session(int userID, const char *role, int sock);
    // int remove_active_session(int sock);
    // int validate_login(const char *role, const char *uid, const char *password);
    // int is_user_active(const char *uid, const char *role);
    // The ID in the session structure is an integer, so we need to convert 'id' (char*) to int.

    int sock = *(int *)client_socket;
    char buffer[BUFFER_SIZE], role[20], id[10], password[50];
    int bytes_read;
    int user_id_int; // Integer version of the ID

    // Read Role
    bytes_read = read(sock, role, sizeof(role) - 1);
    if (bytes_read <= 0) goto cleanup; // Handle disconnection before login
    role[bytes_read] = '\0';

    // Read Credentials (ID and Password)
    bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) goto cleanup; // Handle disconnection before login
    buffer[bytes_read] = '\0';

    // Parse ID and Password
    sscanf(buffer, "%s %s", id, password);
    user_id_int = atoi(id); // Convert ID string to integer

    char response_buffer[BUFFER_SIZE];
    
    // --- STEP 1: Check for Active Session ---
    if (is_user_active(id, role)) {
        strcpy(response_buffer, "ALREADY_LOGGED_IN");
        send(sock, response_buffer, strlen(response_buffer), 0);
        goto cleanup; // End thread if already logged in
    }

    // --- STEP 2: Validate Credentials ---
    if (validate_login(role, id, password))
    {
        // --- STEP 3: LOGIN SUCCESSFUL - RECORD SESSION ---
        if (add_active_session(user_id_int, role, sock)) {
            // Session added successfully
            write(sock, "SUCCESS", strlen("SUCCESS"));

            // --- STEP 4: Handle Client Requests based on Role ---
            if (strcmp(role, "customer") == 0)
            {
                handle_customer_requests(sock, id); 
            }
            else if (strcmp(role, "employee") == 0)
            {
                handle_employee_requests(sock, id);
            }
            else if (strcmp(role, "manager") == 0)
            {
                handle_manager_requests(sock, id); 
            }
            else if (strcmp(role, "admin") == 0)
            {
                handle_admin_requests(sock, id);
            }
            // All 'handle_..._requests' functions must exit the loop 
            // and return here when the user logs out or the connection is lost.
            
            // --- STEP 5: REMOVE SESSION AFTER LOGOUT/DISCONNECT ---
            remove_active_session(sock); 
        } else {
            // Failed to add session (e.g., server full)
            write(sock, "SERVER_SESSION_ERROR", strlen("SERVER_SESSION_ERROR"));
        }
    }
    else
    {
        // Login Failed (Invalid Credentials)
        write(sock, "Invalid ID or Password or Role", strlen("Invalid ID or Password or Role"));
    }

cleanup:
    // Final cleanup regardless of where the thread exited (goto target)
    close(sock);
    free(client_socket);
    pthread_exit(NULL);
}


int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("New client connected!\n");

        int *client_sock = (int *)malloc(sizeof(int));
        *client_sock = new_socket;

        pthread_create(&thread_id, NULL, handle_client, (void *)client_sock);
    }

    close(server_fd);
    return 0;
}