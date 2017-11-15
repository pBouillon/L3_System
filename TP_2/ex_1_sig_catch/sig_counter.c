/**
 * \file  sig_counter.c
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


int sigusr_amount = 0 ; /*!< Amount of SIGUSR1 received */

/**
 * \fn void child_handler (int signal) 
 */
void child_handler (int signal)
{
    switch (signal)
    {
        case SIGUSR1:
            ++sigusr_amount ;
            break ;
        case SIGTERM:
            printf (
                "%d - Signal(s) received: %d\n", 
                getpid(),
                sigusr_amount
            ) ;
            exit(EXIT_SUCCESS) ;
            break ;
        default:
            perror("Unexpected error.\n");
            exit(EXIT_FAILURE) ;
            break ;
    }
}

/**
 * \fn void set_mask ()
 */
void set_mask ()
{
    struct sigaction sa ;

    sa.sa_handler = child_handler ;
    sa.sa_flags   = 0 ;
    sigemptyset(&(sa.sa_mask)) ;
    sigaction(SIGUSR1, &sa, NULL) ;
    sigaction(SIGTERM, &sa, NULL) ;
}

/**
 * \fn sig_counter.c main
 */
int main (int argc, char const *argv[])
{
    printf("%d - Now polling ...\n", getpid()) ;
    fflush(stdout) ;
    
    set_mask() ;
    for(;;) {}

    return 0 ;
}
