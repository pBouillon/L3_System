/**
 * \file  server.c
 * \brief TP3 exercise 1
 *
 * \author Pierre Bouillon
 * \version 0.0.1
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "comm.h"

int queue_id ;

/**
 *
 */
int abort_server (char* msg, int err_id) 
{
    fprintf (stderr, "\n%s\n", msg) ;
    exit (err_id) ;
}

/**
 *
 */
void destruct (int signal)
{
    printf ("%s\n", "Server: Exciting...") ;
    msgctl(queue_id, IPC_RMID, 0) ;
}

/**
 *
 */
int init_queue (key_t key) 
{
    return msgget (
              key,
              IPC_CREAT | 0660 | IPC_EXCL  
            ) ;
}

/**
 *
 */
key_t get_key () 
{
    return ftok(SOURCE_FILE, PROJECT_ID) ;
}

/**
 *
 */
int get_queue_num (int key) 
{
    return msgget(key, 0) ;
}

/**
 *
 */
int read_message (int queue_num, payload *buff) 
{
    return msgrcv (
            queue_num,
            buff,
            sizeof(payload) - sizeof(long),
            CLIENT_MSG_TYPE, 
            0
        ) ;
}

/**
 *
 */
int send_message (int queue_num, int val) 
{
    payload buffer ;
    
    buffer.mtype = SERVER_MSG_TYPE ;
    buffer.value = val ;

    return msgsnd (
            queue_num, 
            &buffer, 
            sizeof(payload) - sizeof(long), 
            MSG_GET_FLAGS
        ) ;
}

/**
 */
void set_mask ()
{
    struct sigaction sa ;

    sa.sa_handler = destruct ;
    sa.sa_flags   = 0 ;
    sigemptyset (&(sa.sa_mask)) ;
    sigaction (SIGINT, &sa, NULL) ;
}



/* main */
int main (int argc, char const *argv[])
{
    key_t   key ;
    payload buffer ;

    // setting handler for SIGINT
    set_mask () ;

    // getting key
    if ((key = get_key()) == -1) 
    {
        abort_server (
            "Unable to generate the key.", 
            ERR_KEY_GEN
        ) ;
    }  

    // creating queue
    if (init_queue (key) == -1) 
    {
        abort_server (
            "Unable to create the queue.", 
            ERR_GET_QUEUE
        ) ;
    }

    // getting queue ID
    if ((queue_id = get_queue_num(key)) == -1)
    {
        abort_server (
            "Non-existing queue.", 
            ERR_GET_QUEUE
        ) ;
    }


    printf ("%s\n", "Server: Now running.") ;
    for (;;) 
    {
        // reading client's message
        if (read_message (queue_id, &buffer) == -1)
        {
            abort_server (
                "Unable to get queue's response.", 
                ERR_QUEUE_RESP
            ) ;
        }

        printf (
            "%s\n", 
            "Server: Value received! Sending response..."
        ) ;

        // sending response
        if (send_message (queue_id, buffer.value * 2) == -1)
        {
            abort_server (
                "Unable to send value in the queue.", 
                ERR_SEND_VAL
            ) ;
        }
        printf ("%s\n", "Server: Response sent !") ;
    }

    return 0 ;
}
