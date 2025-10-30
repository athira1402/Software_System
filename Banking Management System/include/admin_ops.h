#ifndef ADMIN_OPS_H
#define ADMIN_OPS_H 

int add_employee(int uid, const char *fname, const char *lname, const char *pwd) ;
int delete_employee(int employeeID, int sock) ;
void promote_to_manager(int employeeID, int sock) ;
void demote_to_employee(int managerID, int sock) ;
int change_admin_password(int uid, const char *new_password) ;  
void manage_user_roles(int sock) ;  

#endif // ADMIN_OPS_H