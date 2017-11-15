/**
 * \file  fork_execl.h
 * \brief Third exercise of the first TP - header - FST L3
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */

#ifndef FORK_EXECL_H_
#define FORK_EXECL_H_

/**
 * \fn void complex (char *argv[]) *
 * \brief create a child, the child call another program with
 *        the father *argv[], then the father exit after the child
 *
 * \param *argv[] : (**char) program arguments
 */
void complex (char *argv[]) ;

/**
 * \fn void simple ()
 * \brief create one son and print the current PID + the son's PID
 *        then exit them all
 */
void simple () ;

#endif
