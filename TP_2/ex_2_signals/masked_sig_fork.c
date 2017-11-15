/**
 * \file  masked_sig_fork.c
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
 * \brief sig_handler header
 */
void sig_handler () ;


/**
 * \fn void covering ()
 * \brief create one child
 */
void covering () 
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
                "./covering", 
                "covering", 
                (char *)NULL
            ) ;
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        sleep(1) ;
        kill (pid, SIGINT) ;
        kill (pid, SIGTERM) ;
        waitpid(pid, &status, 0) ;
        switch(status)
        {
            case SIGINT:
                printf("%s\n", "SIGINT catched\n") ;
                break ;
            case SIGTERM:
                printf("%s\n", "SIGTERM catched\n") ;
                break ;
            default:
                fprintf(stderr, "%s\n", "Unhandled exception\n") ;
        }
        exit(EXIT_SUCCESS) ;
    }
}

/**
 * \fn void non_covering ()
 * \brief create one child
 */
void non_covering () 
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
        sleep(2) ;
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        sleep(1) ;
        kill (pid, SIGINT) ;
        kill (pid, SIGTERM) ;
        waitpid(pid, &status, 0) ;
        switch(status)
        {
            case SIGINT:
                printf("%s\n", "SIGINT catched\n") ;
                break ;
            case SIGTERM:
                printf("%s\n", "SIGTERM catched\n") ;
                break ;
            default:
                fprintf(stderr, "%s\n", "Unhandled exception\n") ;
        }
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
 * \brief masked_sig_fork.c main
 */
int main(int argc, char const *argv[])
{
    set_mask() ;

    printf("%s\n", "First way :") ;
    non_covering () ;
    printf("%s\n", "Second way :") ;
    covering () ;

    return 0;
}
    