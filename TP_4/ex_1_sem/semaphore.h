#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#define PROJECT_ID      5
#define PROJECT_FILE    "./semaphore.c"

int P (int sem_id, int sem_num) ;
int V (int sem_id, int sem_num) ;

int sem_create (int* sem_id, int sem_num) ;
int sem_destroy (int sem_id) ;
int sem_init (int sem_id, int sem_num, int tokens) ;
int sem_list (int* sem_id, int sem_num) ;

int get_key () ;

#endif