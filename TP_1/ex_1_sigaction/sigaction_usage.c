/**
 * \file  sigaction_usage.c
 * \brief First exercise of the first TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sigaction_usage.h"

#define EXIT_TRIGGER     2    /*!< SIGINTs needed before exit */
#define HOLDING_DURATION 500  /*!< Time between each displays */
#define MS_TO_S          1000 /*!< Factor between ms and s    */

int interruption_count = 0 ;  /*!< counts SIGINTs */

/**
 * \fn void alarm_handler (int signal)
 * \brief catch signals and handle them
 *
 * \param signal : (int) signal received
 */
void alarm_handler (int signal) 
{
    switch (signal)
    {
        case SIGINT:
            printf("%s\n", "Process interrupted.");
            fflush(stdout) ;
            ++interruption_count ;
            break ;
        default:
            perror("Unexpected error.\n");
            exit(EXIT_FAILURE) ;
            break ;
    }

    if (interruption_count == EXIT_TRIGGER)
    {
        perror("Interruption.\n") ;
        exit(EXIT_FAILURE) ;
    }
}

/**
 * \fn void count_from_to (int max)
 * \brief count from `start` to `max`
 *
 * \param max : (int) bound to reach
 */
void count_from_to (int start, int max)
{
    for (int i = start; i <= max; ++i)
    {
        printf("Timer: %d\n", i) ;
        fflush(stdout) ;
        usleep (ms2us (HOLDING_DURATION)) ;
    }
}

/**
 * \fn int ms2us (int ms)
 * \brief Convert a given time in ms into seconds
 *
 * \param ms : (int) milliseconds provided
 * \return (int) time in seconds
 */
int ms2us (int ms)
{
	return ms * 1000 ;
}

/**
 * \fn void first_timer () 
 * \brief First part of the exercise
 */
void first_timer () 
{
    set_mask() ; 
    count_from_to(1, 60) ;
}

/**
 * \fn void second_timer () 
 * \brief Second part of the exercise
 */
void second_timer ()
{
    sigset_t sa, sa_saved ;

    sigaddset(&sa, SIGINT) ;
    sigprocmask(SIG_SETMASK, &sa, &sa_saved) ;

    count_from_to(1, 40) ;

    printf("%s\n", "\n***\nuninstalled mask\n***\n") ;
    fflush(stdout) ;

    sigprocmask(SIG_SETMASK, &sa_saved, NULL) ;

    count_from_to(41, 60) ;
}

/**
 * \fn void set_mask ()
 * \brief Set a mask to catch SIGINT
 */
void set_mask ()
{
    struct sigaction sa ;

    sa.sa_handler = alarm_handler ;
    sa.sa_flags   = 0 ;
    sigemptyset(&(sa.sa_mask)) ;
    sigaction(SIGINT, &sa, NULL) ;
}

/**
 * \brief sigaction_usage.c main
 */
int main (int argc, char const *argv[])
{
    // first_timer()
    second_timer() ;
    return 0 ;
}
