/**
 * \file  sleeping_prog.c
 * \brief Second exercise of the second TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <unistd.h>

#define DURATION 10 /*!< Holding duration */

/**
 * \brief sleeping_prog.c main
 */
int main(int argc, char const *argv[])
{
    sleep (DURATION) ;
    return 0;
}
