#include "bank.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// Lock a customer record
void lock_record(int fd, int recno, short lock_type) {
    struct flock fl;
    fl.l_type = lock_type;  // F_RDLCK / F_WRLCK
    fl.l_whence = SEEK_SET;
    fl.l_start = recno * sizeof(struct Customer);
    fl.l_len = sizeof(struct Customer);
    fl.l_pid = getpid();
    fcntl(fd, F_SETLKW, &fl);
}

// Unlock a customer record
void unlock_record(int fd, int recno) {
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = recno * sizeof(struct Customer);
    fl.l_len = sizeof(struct Customer);
    fl.l_pid = getpid();
    fcntl(fd, F_SETLK, &fl);
}
