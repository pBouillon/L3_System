/**
 * \file  recovering.c
 * \brief Third exercise of the first TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * \brief recovering.c main
 */
int main (int argc, char const *argv[])
{
    if (argc != 3)
    {
        perror("Bad arg number\n") ;
        exit(EXIT_FAILURE) ;
    }
    printf (
        "Operation result: %d %s.\n", 
        (atoi(argv[0]) * atoi(argv[1])), 
        argv[2]
    ) ;
    fflush(stdout) ;
    return 0;
}
