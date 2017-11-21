#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define INCREMENTS              100000 
#define SHARED_VAR_INIT_VAL     0 

#define FATHER_STEP             3
#define CHILD_STEP              3

int  abort_prog (char* msg, int err_id) ;

void create_child () ;

void increment (int step) ;

void init_vars () ;

#endif
