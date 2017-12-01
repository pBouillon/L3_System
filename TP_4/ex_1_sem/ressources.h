#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define INCREMENTS              100000
#define SHARED_VAR_INIT_VAL     0 

#define FATHER_STEP             3
#define CHILD_STEP              5


int  abort_prog (char* msg, int err_id) ;
void create_child () ;
void sem_create_child () ;
void increment (int step) ;
void sem_increment(int step, int sem_num, int sem_id) ;
void init_vars () ;

#endif
