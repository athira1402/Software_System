#ifndef CUSTOMER_OPS_H
#define CUSTOMER_OPS_H

float get_balance(int uid);
int deposit(int uid, float amount);
int withdraw(int uid, float amount);
int transfer(int from_uid, int to_uid, float amount);
int apply_loan(int uid, float amount) ;
int change_cust_password(int uid, const char *new_password) ;
int add_feedback(int customerID, const char *message);
int view_transaction_history(int sock, int userID) ;
void handle_logout(int client_sock) ;

#endif // CUSTOMER_OPS_H
