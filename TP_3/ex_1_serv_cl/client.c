/**
 * \file  client.c
 * \brief TP3 exercise 1
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "comm.h"


/**
 *  \fn abort_client
 */
int abort_client (char* msg, int err_id) 
{
    fprintf (stderr, "%s\n", msg) ;
    exit (err_id) ;
} /* abort_client (char* msg, int err_id) */

/**
 *  \fn get_key
 */
key_t get_key () 
{
    return ftok(SOURCE_FILE, PROJECT_ID) ;
} /* get_key () */

/**
 *  \fn get_queue_num
 */
int get_queue_num (int key) 
{
    return msgget(key, 0) ;
} /* get_queue_num (int key) */

/**
 *  \fn read_message
 */
int read_message (int queue_num, payload *buff) 
{
    return msgrcv (
            queue_num,
            buff,
            sizeof(payload) - sizeof(long),
            SERVER_MSG_TYPE, 
            0
        ) ;
} /* read_message (int queue_num, payload *buff) */

/**
 *  \fn send_message
 */
int send_message (int queue_num, int val) 
{
    payload buffer ;
    
    buffer.mtype = CLIENT_MSG_TYPE ;
    buffer.value = val ;

    return msgsnd (
            queue_num, 
            &buffer, 
            sizeof(payload) - sizeof(long), 
            MSG_GET_FLAGS
        ) ;
} /* send_message (int queue_num, int val) */


/* main */
int main (int argc, char const *argv[])
{
    int     queue_id ;
    int     value ;
    key_t   key ;
    payload buffer ;

    // getting the value 
    if (argc != 2)
    {
        abort_client ("Bad arg number.", ERR_ARG) ;
    }
    value = atoi(argv[1]) ;

    // getting the key
    if ((key = get_key()) == -1) 
    {
        abort_client ("Unable to generate the key.", ERR_KEY_GEN) ;
    }   

    // getting queue ID
    if ((queue_id = get_queue_num(key)) == -1)
    {
        abort_client ("Non-existing queue.", ERR_GET_QUEUE) ;
    }

    // sending value
    if (send_message (queue_id, value) == -1)
    {
        abort_client ("Unable to send value in the queue.", ERR_SEND_VAL) ;
    }

    // reading server's return
    if (read_message (queue_id, &buffer) == -1)
    {
        abort_client ("Unable to get queue's response.", ERR_QUEUE_RESP) ;
    }

    // display result
    printf("Server returned: %d\n", buffer.value) ;

    return 0 ;
}
