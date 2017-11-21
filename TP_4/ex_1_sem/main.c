/**
 * \file  main.c
 * \brief First exercise of the fourth TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ressources.h"

int* p_shared_variable ;


/**
 *  \fn abort_prog
 */
int abort_prog (char* msg, int err_id) 
{
    fprintf (stderr, "%s\n", msg) ;
    exit (err_id) ;
} /* abort_prog (char* msg, int err_id) */

/**
 * \fn void create_child ()
 * \brief create one child
 */
void create_child () 
{
    int pid, status ;

    if ((pid = fork()) < 0) 
    {
        abort_prog("Unable to perform `fork`", EXIT_FAILURE) ;
    }
    // Child process
    else if (pid == 0) 
    {
        increment (CHILD_STEP) ;
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        sleep(1) ;

        increment (FATHER_STEP) ;

        kill (pid, SIGTERM) ;
        waitpid(pid, &status, 0) ;
    }
}

/**
 *
 */
void increment (int step) 
{
    int i ;

    for (i = 0; i < INCREMENTS; ++i)
    {
        int tmp = (*p_shared_variable) ;
        tmp += step ;
        (*p_shared_variable) = tmp ;
    }
}

/**
 *  void \fn init_vars ()
 */
void init_vars () 
{
    int shared_variable ;

    shared_variable   = SHARED_VAR_INIT_VAL ;
    p_shared_variable = &shared_variable  ;
} /* init_vars () */

int main(int argc, char const *argv[])
{
    init_vars () ;
    create_child() ;

    printf (
            "%s%d\n", 
            "shared_variable value: ", 
            (*p_shared_variable)
        );
    p_shared_variable = 0 ;

    return 0;
}
