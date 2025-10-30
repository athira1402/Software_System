#ifndef MANAGER_OPS_H
#define MANAGER_OPS_H

int update_customer_status(int custID, const char *action);
int assign_loan_to_employee(int empID, int loanID);
int change_mng_password(int uid, const char *new_password);

#endif
