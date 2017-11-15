/**
 * \file  fork_execl.c
 * \brief Third exercise of the first TP - FST L3
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

#include "fork_execl.h"

/**
 * \fn void complex (char *argv[]) *
 * \brief create a child, the child call another program with
 *        the father *argv[], then the father exit after the child
 *
 * \param *argv[] : (**char) program arguments
 */
void complex (char *argv[]) 
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
                "./recovering", 
                argv[1], 
                argv[2], 
                argv[3], 
                (char *)NULL
            ) ;
        if ( ret != 0) 
        {
            perror("An error occured on recovering.\n") ;
            exit(EXIT_FAILURE) ;
        }
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        waitpid(pid, &status, 0) ;
        exit(EXIT_SUCCESS) ;
    }
}

/**
 * \fn void simple ()
 * \brief create one son and print the current PID + the son's PID
 *        then exit them all
 */
void simple () 
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
        printf("Son's pid: %d\n", getpid()) ;
        fflush(stdout) ;
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        printf("Father's pid: %d\n", getpid()) ;
        waitpid(pid, &status, 0) ;
        exit(EXIT_SUCCESS) ;
    }
}

/**
 * \brief fork_execl.c main
 */
int main (int argc, char *argv[])
{   
    // part 1 
    // simple() ;

    // part 2
    // program's name + arg1 + arg2 + str = 4
    if (argc != 4) 
    {
        perror ("Incorrect arg number: ./prog int1 int2 name\n") ;
        exit(EXIT_FAILURE) ;
    }
    complex (argv) ;
    return 0 ;
}
