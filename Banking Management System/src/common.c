#include "../include/client_io.h"

void send_request(int sock, const char *request) 
{
    if (write(sock, request, strlen(request)) < 0) 
    {
        perror("Write failed");
        close(sock);
        exit(1); 
    }
}


void receive_response(int sock, char *buffer, size_t buffer_size) 
{
    // Use read() for block-by-block response as per original logic
    int bytes_read = read(sock, buffer, buffer_size - 1); // -1 for null terminator
    if (bytes_read < 0) 
    {
        perror("Error receiving data from server");
        close(sock);
        exit(1);
    }
    buffer[bytes_read] = '\0'; 
}


void receive_tresponse(int sock) 
{
    char buffer[BUFFER_SIZE];

    while (1) 
    {
        // Use recv() for stream reading as per original logic
        ssize_t bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) 
        {
            // If bytes is 0, connection was closed gracefully. If < 0, it's an error.
            if (bytes < 0) {
                perror("Error receiving data");
            } else {
                printf("\nServer closed connection.\n");
            }
            break;
        }
        buffer[bytes] = '\0';
        
        // Check for the termination sequence
        if (strcmp(buffer, "END") == 0) 
            break; 
            
        printf("%s", buffer); 
    }
}
