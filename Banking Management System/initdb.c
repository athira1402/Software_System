#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "struct/customer.h"
#include "struct/employee.h"
#include "struct/manager.h"
#include "struct/admin.h"

#include "include/client_io.h"

int initialize_customers() {
    int fd = open(CUSTOMER_DB, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    struct customer c = {1, "Ira", "John","pass123", 5000.0, 0.0, "active"};
    write(fd, &c, sizeof(c));
    struct customer d = {2, "Ezra", "Dsouza","pass456", 3000.0, 1.0, "inactive"};
    write(fd, &d, sizeof(d));
    printf("[INITDB] %d default employee records written to %s.\n", 2, CUSTOMER_DB);
    close(fd);
    return 1;
}

int initialize_employees() {
    struct employee employees[] = {
        {1001, "John", "Lona", "passemp1", "active", {2001, 2002}, 2},
        {1002, "Larry", "Perera", "passemp2", "active", {2003}, 1}
    };
    int num_employees = sizeof(employees) / sizeof(employees[0]);

    int fd = open(EMPLOYEE_DB, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        perror("Error opening employees.dat");
        return 0;
    }

    if (write(fd, employees, num_employees * sizeof(struct employee)) != (ssize_t)(num_employees * sizeof(struct employee))) {
        perror("Error writing to employees.dat");
        close(fd);
        return 0;
    }

    printf("[INITDB] %d default employee records written to %s.\n", num_employees, EMPLOYEE_DB);
    close(fd);
    return 1;
}

int initialize_managers() {
    struct manager managers[] = {
        {2001, "Grace", "Star", "passmgr1"},
        {2002, "Augustus", "paul", "passmgr2"}
    };
    int num_managers = sizeof(managers) / sizeof(managers[0]);
    int fd = open(MANAGER_DB, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening managers.dat");
        return 0;
    }
    if (write(fd, managers, num_managers * sizeof(struct manager)) != (ssize_t)(num_managers * sizeof(struct manager))) {
        perror("Error writing to managers.dat");
        close(fd);
        return 0;
    }

    printf("[INITDB] %d default manager records written to %s.\n", num_managers, MANAGER_DB);
    close(fd);
    return 1;
}

int initialize_admins() {
    struct admin admins[] = {
        {3001, "Athira", "John", "Athira@123"}
    };
    int num_admins = sizeof(admins) / sizeof(admins[0]);
    int fd = open(ADMIN_DB, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        perror("Error opening admins.dat");
        return 0;
    }

    if (write(fd, admins, num_admins * sizeof(struct admin)) != (ssize_t)(num_admins * sizeof(struct admin))) {
        perror("Error writing to admins.dat");
        close(fd);
        return 0;
    }

    printf("[INITDB] %d default admin records written to %s.\n", num_admins, ADMIN_DB);
    close(fd);
    return 1;
}

int main() {
    if (!initialize_customers()) {
        fprintf(stderr, "Initialization of customers failed.\n");
        return 1;
    }
    if (!initialize_employees()) {
        fprintf(stderr, "Initialization of employees failed.\n");
        return 1;
    }
    if (!initialize_managers()) {
        fprintf(stderr, "Initialization of managers failed.\n");
        return 1;
    }
    if (!initialize_admins()) {
        fprintf(stderr, "Initialization of admins failed.\n");
        return 1;
    }   

    printf("\nDatabase initialization complete.\n");
    return 0;

}