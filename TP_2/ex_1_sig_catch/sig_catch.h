/**
 * \file  sig_catch.h
 * \brief First exercise of the second TP - header - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#ifndef SIG_CATCH_H_
#define SIG_CATCH_H_

#define SIGUSR1_SIGNAL_SENT 5000000 /*!< number of SIGUSR1 to send */
#define TEMPORISATION       1       /*!< Time to wait before running */

/**
 * \fn void child_handler (int signal) 
 */
void child_handler (int signal) ;

/**
 * \fn void create_child ()
 * \brief create one child
 */
void create_child () ;

/**
 * \fn void set_mask ()
 */
void set_mask () ;

#endif
