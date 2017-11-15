/**
 * \file  timer.h
 * \brief Second exercise of the first TP - header - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#ifndef TIMER_H_
#define TIMER_H_

/**
 * \struct timer
 * \brief  handle all the timer's values
 */
typedef struct timer 
{
    unsigned int start ; /*!< timestamp value on start     */
    unsigned int stop  ; /*!< timestamp value when stopped */
    int time_left ;      /*!< time elapsed between start and stop */
} 
Timer ;

/**
 * \fn void alarm_handler (int signal)
 * \brief catch signals and handle them
 *
 * \param signal : (int) signal received
 */
void alarm_handler (int signal) ;

/**
 * \fn print_time_left ()
 * \brief prints a message with the remaining time, then exit
 */
void print_time_left () ;

/**
 * \fn void set_mask ()
 * \brief Set a mask to catch SIGINT
 */
void set_mask () ;

/**
 * \fn void start_timer ()
 * \brief set the start of the timer and launch the alarm
 */
void start_timer () ;

/**
 * \fn void stop_timer ()
 * \brief calculate the remaining time and save the time when the 
 *        timer has stopped
 */
void stop_timer () ;

#endif
