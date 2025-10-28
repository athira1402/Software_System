#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "../include/bank.h"
#include "../include/session.h"


int add_customer(int dbfd, const char *name, const char *password, double balance) {
    // Lock the entire file for exclusive access during the count and write process
    // This is crucial to prevent race conditions when multiple employees add customers simultaneously.
    struct flock lock = {0};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Lock the entire file
    fcntl(dbfd, F_SETLKW, &lock);

    // Determine the new Customer ID
    off_t file_size = lseek(dbfd, 0, SEEK_END);
    if (file_size < 0) {
        perror("lseek to determine size failed");
        lock.l_type = F_UNLCK;
        fcntl(dbfd, F_SETLK, &lock);
        return -1;
    }

    int num_records = file_size / sizeof(struct Customer);
    int new_id = num_records + 1; // IDs start at 1

    // Create the new customer record
    struct Customer new_cust;
    new_cust.id = new_id;
    strncpy(new_cust.username, name, NAME_LEN - 1);
    new_cust.username[NAME_LEN - 1] = '\0';
    strncpy(new_cust.password, password, PASS_LEN - 1);
    new_cust.password[PASS_LEN - 1] = '\0';
    new_cust.balance = balance;

    // Append the record to the end of the file (lseek is already at the end)
    if (write(dbfd, &new_cust, sizeof(struct Customer)) != sizeof(struct Customer)) {
        perror("write new customer failed");
        new_id = -1; // Indicate failure
    }
    
    // Unlock the file
    lock.l_type = F_UNLCK;
    fcntl(dbfd, F_SETLK, &lock);
    
    return new_id;
}