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

#define PORT 8080
#define BUFFER_SIZE 1024

int add_employee(int uid, const char *fname, const char *lname, const char *pwd) 
{
    int fd = open(EMPLOYEE_DB, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) 
    {
        perror("Failed to open the employee file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = 0;  

    if (fcntl(fd, F_SETLKW, &lock) == -1) 
    {
        perror("Failed to lock the file");
        close(fd);
        return -1;
    }

    struct employee new_employee;
    new_employee.employeeID = uid;
    strncpy(new_employee.first_name, fname, sizeof(new_employee.first_name) - 1);
    new_employee.first_name[sizeof(new_employee.first_name) - 1] = '\0';
    strncpy(new_employee.last_name, lname, sizeof(new_employee.last_name) - 1);
    new_employee.last_name[sizeof(new_employee.last_name) - 1] = '\0';
    strncpy(new_employee.password, pwd, sizeof(new_employee.password) - 1);
    new_employee.password[sizeof(new_employee.password) - 1] = '\0';
    memset(new_employee.assigned_loans, 0, sizeof(new_employee.assigned_loans));
    new_employee.assigned_loans[sizeof(new_employee.assigned_loans) - 1] = '\0';
    new_employee.loan_count = 0;
    strncpy(new_employee.status, "Active", sizeof(new_employee.status) - 1);
    new_employee.status[sizeof(new_employee.status) - 1] = '\0';

    if (write(fd, &new_employee, sizeof(struct employee)) != sizeof(struct employee)) 
    {
        perror("Failed to write new employee to the file");
        fcntl(fd, F_UNLCK, &lock); 
        close(fd);
        return -1;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    printf("New customer (ID: %d) added successfully.\n", uid);
    return 0; 
}

void delete_employee(int employeeID, int sock) {
    char buffer[BUFFER_SIZE];
    int found = 0;

    FILE *fp = fopen(EMPLOYEE_DB, "rb");
    FILE *temp = fopen(TEMP_EMP_DB, "wb");

    if (!fp || !temp) {
        perror("File error");
        strcpy(buffer, "Error accessing employee data.\n");
        write(sock, buffer, strlen(buffer));
        close(sock);
        return;
    }

    struct employee emp;

    while (fread(&emp, sizeof(struct employee), 1, fp)) 
    {
        if (emp.employeeID == employeeID) 
        {
            found = 1; 
        } 
        else 
        {
            fwrite(&emp, sizeof(struct employee), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) 
    {
        remove(EMPLOYEE_DB);
        rename(TEMP_EMP_DB, EMPLOYEE_DB);
        strcpy(buffer, "Employee deleted successfully.\n");
    } 
    else 
    {
        remove(TEMP_EMP_DB);
        strcpy(buffer, "Employee not found.\n");
    }

    write(sock, buffer, strlen(buffer));
}

void promote_to_manager(int employeeID, int sock) 
{
    FILE *emp_fp = fopen(EMPLOYEE_DB, "rb");
    FILE *mgr_fp = fopen(MANAGER_DB, "ab");
    struct employee emp;
    struct manager mgr;
    char buffer[BUFFER_SIZE];
    int found = 0;

    if (!emp_fp || !mgr_fp) 
    {
        perror("Error opening files");
        strcpy(buffer, "Error accessing data.\n");
        write(sock, buffer, strlen(buffer));
        return;
    }

    FILE *temp_fp = fopen(TEMP_EMP_DB, "wb");
    while (fread(&emp, sizeof(struct employee), 1, emp_fp)) 
    {
        if (emp.employeeID == employeeID) 
        {
            found = 1;
            mgr.managerID = emp.employeeID;
            strncpy(mgr.first_name, emp.first_name, sizeof(mgr.first_name) - 1);
            mgr.first_name[sizeof(mgr.first_name) - 1] = '\0';
            strncpy(mgr.last_name, emp.last_name, sizeof(mgr.last_name) - 1);
            mgr.last_name[sizeof(mgr.last_name) - 1] = '\0';
            strncpy(mgr.password, emp.password, sizeof(mgr.password) - 1);
            mgr.password[sizeof(mgr.password) - 1] = '\0';

            fwrite(&mgr, sizeof(struct manager), 1, mgr_fp);
        } 
        else 
        {
            fwrite(&emp, sizeof(struct employee), 1, temp_fp); 
        }
    }

    fclose(emp_fp);
    fclose(mgr_fp);
    fclose(temp_fp);

    remove(EMPLOYEE_DB);
    rename(TEMP_EMP_DB, EMPLOYEE_DB);

    if (found) 
    {
        strcpy(buffer, "Employee promoted to Manager successfully.\n");
    } 
    else 
    {
        strcpy(buffer, "Employee not found.\n");
    }
    write(sock, buffer, strlen(buffer));
}

void demote_to_employee(int managerID, int sock) 
{
    FILE *mgr_fp = fopen(MANAGER_DB, "rb");
    FILE *emp_fp = fopen(EMPLOYEE_DB, "ab");
    struct manager mgr;
    struct employee emp;
    char buffer[BUFFER_SIZE];
    int found = 0;

    if (!mgr_fp || !emp_fp) 
    {
        perror("Error opening files");
        strcpy(buffer, "Error accessing data.\n");
        write(sock, buffer, strlen(buffer));
        return;
    }

    FILE *temp_fp = fopen("./data/temp_manager.data", "wb");
    while (fread(&mgr, sizeof(struct manager), 1, mgr_fp)) 
    {
        if (mgr.managerID == managerID) 
        {
            found = 1;
            emp.employeeID = mgr.managerID;
            strncpy(emp.first_name, mgr.first_name, sizeof(emp.first_name) - 1);
            emp.first_name[sizeof(emp.first_name) - 1] = '\0';
            strncpy(emp.last_name, mgr.last_name, sizeof(emp.last_name) - 1);
            emp.last_name[sizeof(emp.last_name) - 1] = '\0';
            strncpy(emp.password, mgr.password, sizeof(emp.password) - 1);
            emp.password[sizeof(emp.password) - 1] = '\0';
            strcpy(emp.status, "Active");
            emp.loan_count = 0; 
            memset(emp.assigned_loans, 0, sizeof(emp.assigned_loans));
            emp.assigned_loans[sizeof(emp.assigned_loans) - 1] = '\0';

            fwrite(&emp, sizeof(struct employee), 1, emp_fp);
        } 
        else 
        {
            fwrite(&mgr, sizeof(struct manager), 1, temp_fp); 
        }
    }

    fclose(mgr_fp);
    fclose(emp_fp);
    fclose(temp_fp);

    remove(MANAGER_DB);
    rename("./data/temp_manager.data", MANAGER_DB);

    if (found) 
    {
        strcpy(buffer, "Manager demoted to Employee successfully.\n");
    } 
    else 
    {
        strcpy(buffer, "Manager not found.\n");
    }
    write(sock, buffer, strlen(buffer));
}

void manage_user_roles(int sock) 
{
    char buffer[BUFFER_SIZE];
    int id, found = 0;
    (void)found;
    char new_role[20];

    int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) 
    {
        perror("Error reading ID");
        close(sock);
        return;
    }
    buffer[bytes_read] = '\0';
    sscanf(buffer, "%d", &id); 

    bytes_read = read(sock, new_role, sizeof(new_role) - 1);
    if (bytes_read <= 0) 
    {
        perror("Error reading new role");
        close(sock);
        return;
    }
    new_role[bytes_read] = '\0';

    if (strcmp(new_role, "Manager") == 0) 
    {
        promote_to_manager(id, sock);
    } 
    else if (strcmp(new_role, "Employee") == 0) 
    {
        demote_to_employee(id, sock);
    } 
    else 
    {
        strcpy(buffer, "Invalid role specified.\n");
        write(sock, buffer, strlen(buffer));
    }
}

int change_admin_password(int uid, const char *new_password)
{
    int fd = open(ADMIN_DB, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening customer file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK; 
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; 

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error locking file");
        close(fd);
        return -1;
    }

    struct manager c;
    int found = 0;
    off_t position;

    while (read(fd, &c, sizeof(struct manager)) == sizeof(struct manager))
    {
        if (c.managerID == uid)
        {
            position = lseek(fd, -sizeof(struct manager), SEEK_CUR);
            if (position == -1)
            {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            strncpy(c.password, new_password, sizeof(c.password) - 1);
            c.password[sizeof(c.password) - 1] = '\0';
            if (write(fd, &c, sizeof(struct manager)) != sizeof(struct manager))
            {
                perror("Failed to write updated record");
                flock(fd, LOCK_UN); 
                close(fd);
                return -1;
            }
            found = 1;
            break;
        }
    }

    lock.l_type = F_UNLCK; 
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    if (!found)
    {
        printf("User ID %d not found.\n", uid);
        return -1;
    }

    printf("Password changed successfully.\n");
    return 0;
}

