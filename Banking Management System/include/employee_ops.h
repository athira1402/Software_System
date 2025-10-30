#ifndef EMPLOYEE_OPS_H
#define EMPLOYEE_OPS_H

int add_customer(int uid, const char *fname, const char *lname, const char *pwd, float bal);
int fetch_assigned_loan_details(int *loan_ids, int loan_count, char *details);
int view_employee_loans(int employee_id, char *details);
int update_loan_status(int loan_id, const char *new_status);
int credit_to_customer(int cust_id, float amount);
void view_customer_transactions(int sock);
int change_emp_password(int uid, const char *new_password) ;
void delete_customer(int custID, int sock) ;

#endif // EMPLOYEE_OPS_H