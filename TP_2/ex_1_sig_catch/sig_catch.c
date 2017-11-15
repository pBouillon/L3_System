/**
 * \file  sig_catch.c
 * \brief First exercise of the second TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>

#include "sig_catch.h"

/**
 * \fn void create_child ()
 * \brief create one child
 */
void create_child () 
{
    int pid, status ;

    if ((pid = fork()) < 0) 
    {
        perror("Unable to perform `fork`\n") ;
        exit(EXIT_FAILURE) ;
    }
    // Child process
    else if (pid == 0) 
    {
        int ret ;
        ret = execl(
                "./sig_counter", 
                "sig_counter", 
                (char *)NULL
            ) ;
        if (ret != 0)
        {
            perror("execl failed.\n") ;
            exit(EXIT_FAILURE) ;
        }
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        sleep(1) ;
        printf("sending to %d\n", pid) ;
        for (int i = 0; i < SIGUSR1_SIGNAL_SENT; ++i)
        {
            kill (pid, SIGUSR1) ;
        }
        kill (pid, SIGTERM) ;
        waitpid(pid, &status, 0) ;
        exit(EXIT_SUCCESS) ;
    }
}

/**
 * \brief sig_catch.c main
 */
int main (int argc, char *argv[])
{   
    create_child () ;
    return 0 ;
}
