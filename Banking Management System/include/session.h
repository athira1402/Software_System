#ifndef SESSION_H
#define SESSION_H

#define MAX_SESSIONS 100 // Maximum number of concurrent users
#define MAX_ROLE_LEN 20

// Structure to track an active session
typedef struct {
    int userID;
    char role[MAX_ROLE_LEN];
    int socket_descriptor; // The socket associated with this session
    int is_active;         // Flag to show if the slot is occupied
} ActiveSession;

// Global array to hold all active sessions
ActiveSession active_sessions[MAX_SESSIONS];

#endif // SESSION_H