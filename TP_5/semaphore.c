/**
 * \file  semaphore.c
 * \brief First exercise of the fifth TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>    
#include <sys/types.h>

#include "semaphore.h"

/**
 * \fn int P (int sem_id, int sem_num) 
 */
int P (int sem_id, int sem_num) 
{
    struct sembuf smbf ;
    smbf.sem_num = sem_num ;
    smbf.sem_op  = -1 ;
    smbf.sem_flg =  0 ;
    
    return semop (sem_id, &smbf, 1) ;

} /* int P (int sem_id, int sem_num)  */

/**
 * \fn int V (int sem_id, int sem_num) 
 */
int V (int sem_id, int sem_num) 
{
    struct sembuf smbf ;
    smbf.sem_num = sem_num ;
    smbf.sem_op  = 1 ;
    smbf.sem_flg = 0 ;

    return semop (sem_id, &smbf, 1) ;
} /* int V (int sem_id, int sem_num) */

/**
 * \fn int sem_create (int* sem_id, int sem_num)
 */
int sem_create (int* sem_id, int sem_num) 
{
    *sem_id = semget (
                get_key (),
                sem_num, 
                IPC_CREAT|0660|IPC_EXCL
            ) ;

    return *sem_id < 0 ;    
} /* int sem_create (int* sem_id, int sem_num) */

/**
 * \fn int sem_destroy (int sem_id) 
 */
int sem_destroy (int sem_id) 
{
    return semctl (sem_id, IPC_RMID, 0) ;
} /* int sem_destroy (int sem_id)  */

/**
 * \fn int sem_init (int sem_id, int sem_num, int tokens) 
 */
int sem_init (int sem_id, int sem_num, int tokens) 
{
    return semctl (sem_id, sem_num, SETVAL, tokens) ;
} /* int sem_init (int sem_id, int sem_num, int tokens) */

/**
 * \fn int sem_list (int* sem_id, int sem_num) 
 */
int sem_list (int* sem_id, int sem_num) 
{
    *sem_id = semget(get_key(), sem_num, 0) ;
    return *sem_id < 0 ;
} /* int sem_list (int* sem_id, int sem_num) */

/**
 * int get_key () 
 */
key_t get_key () 
{
    return ftok (PROJECT_FILE, PROJECT_ID) ;
} /* int get_key () */
