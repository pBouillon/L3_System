#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ressources.h"

#ifdef DEBUG
    # define DEBUG_PRINT(x) printf x
#else
    # define DEBUG_PRINT(x) do {} while (0)
#endif

#define  ARGS_NEEDED  5


int  seg_id ;
int* shared_rep ;


/**
 *  \fn abort_prog
 */
int abort_prog (char* msg, int err_id) 
{
    fprintf (stderr, "%s\n", msg) ;
    exit (err_id) ;
} /* abort_prog */

/**
 *
 */
key_t gen_key() {
    return ftok(KEY_FILE, KEY_ID) ;
} /* gen_key */

/**
 * \fn void create_child
 * \brief create one child
 */
void create_child (char *source_file, char *output) 
{
    pid_t pid ;
    int   status ;

    if ((pid = fork()) < 0) 
    {
        fprintf(stderr, "%s\n", "Unable to perform `fork`.");
        exit(EXIT_FAILURE) ;
    }
    // Child process
    else if (pid == 0) 
    {
        int ret ;
        ret = execl (
                "./file_parser", 
                "file_parser", 
                source_file, 
                output,
                NO_OPT,
                NO_OPT,
                (char *)NULL
            ) ;
        DEBUG_PRINT(("DEBUG MAIN -- %s\n", "CHILD: execl done")) ;
        if (ret != 0)
        {
            fprintf(stderr, "%s\n", "`execl` failed.");
            exit(EXIT_FAILURE) ;
        }
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        int l_tot = 0 ;

        waitpid(pid, &status, 0) ;
        DEBUG_PRINT(("DEBUG MAIN -- %s\n", "FATHER: Child terminated")) ;

        DEBUG_PRINT(("DEBUG MAIN -- %s\n", "FATHER: Receiving ...")) ;
        for (int i = 0; i < WORDS_LEN; ++i)
        {
            l_tot += shared_rep[i] ;
        }

        // Displays results as : X sentence[s] of [0]Y word[s]
        for (int i = 0; i < WORDS_LEN; ++i)
        {
            printf (
                "%d sentence", 
                shared_rep[i]
            ) ;
            printf (
                "%s ", 
                (shared_rep[i] > 0) ? "s" 
                                    : " "
            ) ;
            printf (
                "of %s%d ", 
                (i >= 10) ? "" 
                         : "0"
                , i
            ) ;
            printf (
                "%s\n", 
                (i > 0) ? "words" 
                        : "word"
            ) ;
        }
    }
} /* create_child */

/**
 *  \fn init_vars
 */
void init_vars () 
{
    seg_id = shmget (gen_key(), sizeof(int) * WORDS_LEN, IPC_CREAT|0660) ;
    if (seg_id < 0) 
    {
        abort_prog (
            "Failed to create a shared memory space", 
            EXIT_FAILURE
        ) ;
    }
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Shared memory created")) ;
    shared_rep = (int*) shmat (seg_id, NULL, 0) ;
    if (shared_rep == NULL) 
    {
        abort_prog (
            "Unable to get a pointer on shared memory",
            EXIT_FAILURE
        ) ;
    }
    *shared_rep = 0 ;
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Shared memory initialized")) ;
} /* init_vars */

/**
 * \fn print_usage
 */
void print_usage (int count)
{
    fprintf (
        stderr, 
        "%s \n(given: %d)\n", 
        "usage: ./prog <source> <destination> <begin> <rows>",
        count
    ) ;
    exit (EXIT_FAILURE) ;
} /* print_usage */

/**
 *
 */
int main (int argc, char const *argv[])
{
    if (argc != ARGS_NEEDED) 
    {
        print_usage(argc) ;
    }

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Initializing Segment...")) ;
    init_vars() ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Operating fork...")) ;
    create_child ((char*)argv[1], (char*)argv[2]) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Deleting Segment...")) ;
    shmdt(shared_rep) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Exciting...")) ;
    return 0 ;
} /* main */
