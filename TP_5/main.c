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
 * \fn void create_child ()
 * \brief create one child
 */
void create_child (char *source_file, char *output) 
{
    int   status ;
    pid_t pid ;

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
        waitpid(pid, &status, 0) ;
        exit(EXIT_SUCCESS) ;
    }
} /* create_child */

/**
 *  void \fn init_vars
 */
void init_vars () 
{
    int shared_variable ;

    seg_id = shmget (IPC_PRIVATE, sizeof(int) * WORDS_LEN, IPC_CREAT|0660) ;
    if (seg_id < 0) 
    {
        abort_prog (
            "Failed to create a shared memory space", 
            EXIT_FAILURE
        ) ;
    }

    shared_rep = (int*) shmat (seg_id, NULL, 0) ;
    if (shared_rep == NULL) 
    {
        abort_prog (
            "Unable to get a pointer on shared memory",
            EXIT_FAILURE
        ) ;
    }

    shared_rep[0] = 0 ;

} /* init_vars */

/**
 *
 */
int main(int argc, char const *argv[])
{
    DEBUG_PRINT(("%s\n", "Initializing Segment...")) ;
    init_vars() ;
    DEBUG_PRINT(("%s\n", "Operating fork...")) ;
    create_child ((char*)argv[1], (char*)argv[2]) ;
    DEBUG_PRINT(("%s\n", "Deleting Segment...")) ;
    shmdt(shared_rep) ;
    DEBUG_PRINT(("%s\n", "Exciting...")) ;
    return 0 ;
} /* main */
