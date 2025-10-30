#include <stdio.h>
#include <stdlib.h>
#include "struct/loan.h"
#define LOAN_DB "data/loan.dat"

int main() {
    FILE *fp = fopen(LOAN_DB, "rb");
    if (!fp) {
        perror("Error opening loan.dat");
        return 1;
    }

    struct loan ln;
    printf("Loan Records:\n");
    printf("-------------------------------\n");

    while (fread(&ln, sizeof(struct loan), 1, fp) == 1) {
        printf("User ID: %d\n", ln.userID);
        printf("Amount: %.2f\n", ln.amount);
        printf("Status: %s\n", ln.status);
        printf("-------------------------------\n");
    }

    fclose(fp);
    return 0;
}
