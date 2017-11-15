/**
 * \file  timer.c
 * \brief Second exercise of the first TP - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "timer.h"

#define TIMER_LIMIT 10 /*!< Seconds before triggering SIGALRM */

Timer timer ;          /*!< Global timer for the game */


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
            stop_timer() ;
            print_time_left() ;
            break ;
        case SIGALRM:
            perror("\nOut of time.\n") ;
            exit(EXIT_FAILURE) ;
            break ;
        default:
            perror("\nUnexpected error.\n") ;
            exit(EXIT_FAILURE) ;
            break ;
    }
}

/**
 * \fn print_time_left ()
 * \brief prints a message with the remaining time, then exit
 */
void print_time_left ()
{ 
    char err_msg[33] ;
    sprintf(
        err_msg, 
        "\nToo soon! Left: %d seconds.\n", 
        timer.time_left
    ) ;

    perror(err_msg) ;
    exit(EXIT_FAILURE) ;
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
    sigaction(SIGINT,  &sa, NULL) ;
    sigaction(SIGALRM, &sa, NULL) ;
}

/**
 * \fn void start_timer ()
 * \brief set the start of the timer and launch the alarm
 */
void start_timer ()
{
    timer.start = time(NULL) ;
    alarm(TIMER_LIMIT) ;
}

/**
 * \fn void stop_timer ()
 * \brief calculate the remaining time and save the time when the 
 *        timer has stopped
 */
void stop_timer ()
{
    timer.stop = time(NULL) ;
    timer.time_left = TIMER_LIMIT ;
    timer.time_left-= (timer.stop - timer.start) ;
}

/**
 * \brief timer.c main
 */
int main (int argc, char const *argv[])
{
    printf("%s\n", "Now starting ! Pending ...") ;
    fflush(stdout) ;

    set_mask() ;

    Timer timer ;
    start_timer() ;
    pause() ;
    
    return 0 ;
}
