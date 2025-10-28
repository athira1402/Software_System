#include <stdio.h>
#include "../include/bank.h"

int main() {
    // Create some dummy customers
    struct Customer customers[] = {
        {1, "Irine","12345", 5000.0},
        {2, "Ishee", "4321", 10000.0},
        {3, "Innie", "pass", 7500.0}
    };

    // Open file in binary write mode (overwrites existing file)
    FILE *fp = fopen("data/employees.dat", "wb");
    if (!fp) {
        perror("Cannot open customers.dat");
        return 1;
    }

    // Write each customer to the file
    for (int i = 0; i < 3; i++) {
        fwrite(&customers[i], sizeof(struct Customer), 1, fp);
    }

    fclose(fp);
    printf("Dummy customer records added successfully!\n");

    return 0;
}
