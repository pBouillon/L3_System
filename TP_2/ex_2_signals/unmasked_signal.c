/**
 * \file  unmasked_signal.c
 * \brief Second exercise of the second TP - FST L3
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

/**
 * \fn void sig_handler (int signal) 
 */
void sig_handler (int signal)
{
    switch (signal)
    {
        case SIGINT:
            printf("%s\n", "SIGINT received") ;
            break ;
        default:
            perror("Unexpected error.\n") ;
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

    sa.sa_handler = sig_handler ;
    sa.sa_flags   = SA_NODEFER ;
    sigemptyset(&(sa.sa_mask)) ;
    sigaction(SIGINT, &sa, NULL) ;
}

/**
 * \brief unmasked_signal.c main
 */
int main(int argc, char const *argv[])
{
    set_mask () ;
    kill(getpid(), SIGINT) ;
    return 0;
}
