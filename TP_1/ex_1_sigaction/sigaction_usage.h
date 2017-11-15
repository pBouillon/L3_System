/**
 * \file  sigaction_usage.h
 * \brief First exercise of the first TP - header - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#ifndef SIGACTION_USAGE_H_
#define SIGACTION_USAGE_H_

/**
 * \fn void alarm_handler (int signal)
 * \fn brief catch signals and handle them
 *
 * \param signal : (int) signal received
 */
void alarm_handler (int signal) ;

/**
 * \fn void count_to (int max)
 * \brief count from 1 to `max`
 *
 * \param max : (int) bound to reach
 */
void count_from_to (int start, int max) ;

/**
 * \fn int ms2us (int ms)
 * \brief Convert a given time in ms into seconds
 *
 * \param ms : (int) milliseconds provided
 * \return (int) time in seconds
 */
int  ms2us (int ms) ;

/**
 * \fn void first_timer () 
 * \brief First part of the exercise
 */
void first_timer () ;

/**
 * \fn void second_timer () 
 * \brief Second part of the exercise
 */
void second_timer () ;

/**
 * \fn void set_mask ()
 * \brief Set a mask to catch SIGINT
 */
void set_mask () ;

#endif
