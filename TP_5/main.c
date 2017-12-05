#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ressources.h"
#include "semaphore.h"

#ifdef DEBUG
    # define DEBUG_PRINT(x) printf x
#else
    # define DEBUG_PRINT(x) do {} while (0)
#endif

#define  ARGS_NEEDED  4
#define  MAX_FORKS    50

int  sem_id ;
int* shared_rep ;


/**
 *  \fn abort_prog
 */
int abort_prog (char* msg, int err_id) 
{
    shmdt(shared_rep) ;
    sem_destroy(SEM_ID) ;

    fprintf (stderr, "%s\n", msg) ;
    exit (err_id) ;
} /* abort_prog */

/**
 *
 */
key_t gen_key() 
{
    return ftok(KEY_FILE, KEY_ID) ;
} /* gen_key */

/**
 * \fn void create_child
 * \brief create one child
 */
void create_child (char *source_file, char *output, int processes) 
{
    int   status ;
    pid_t pid[processes] ;

    int lines, step ;
    int begin, end  ;

    if (processes == 0)
    {
        ++processes ;
    }

    lines = get_file_lines(source_file) ;
    step  = (int)lines/processes ;
    DEBUG_PRINT (("Splitting in %d lines_n\n", step)) ;
    begin = end = 0 ;

    for (int i = 0; i < processes; ++i) 
    {
        begin = end ;
        if (i == processes - 1) 
        {
            end = 0 ;
        }
        else 
        {
            end  += step ;
        }

        if ((pid[i] = fork()) < 0) {
            abort_prog ("Unable to perform `fork`.", EXIT_FAILURE) ;
        } 

        else if (pid[i] == 0) 
        {
            int  ret ;
            char buff_b[4] ;
            char buff_e[4] ;

            DEBUG_PRINT (("CHILD %d: b=%d e=%d\n", i, begin, end)) ;

            snprintf(buff_b, 3, "%d", begin) ;
            snprintf(buff_e, 3, "%d", end) ;

            P (sem_id, i) ;
            ret = execl (
                "./file_parser", 
                "file_parser", 
                source_file, 
                output,
                buff_b,
                buff_e,
                (char *)NULL
            ) ;
            V (sem_id, i) ;

            DEBUG_PRINT(("DEBUG MAIN -- (%d) %s\n", i, "CHILD: execl done")) ;

            if (ret != 0) 
            {
                abort_prog ("`execl` failed.", EXIT_FAILURE) ;
            }
            exit(EXIT_SUCCESS) ;
        }
    }

    // Parent process
    int l_tot = 0 ;

    for (int i = 0; i < processes; ++i)
    {
        waitpid(pid[i], &status, 0) ;
        if (status != EXIT_SUCCESS) 
        {
            abort_prog ("Child excited with an error", EXIT_FAILURE) ;
        }
    }
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "FATHER: Childs terminated")) ;

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
            (shared_rep[i] > 1) ? "s" 
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
            (i > 1) ? "words" 
                    : "word"
        ) ;
    }
} /* create_child */

/**
 *
 */
int get_file_lines (char* filename)
{
    FILE *file ;
    char  buff[BUFF_SIZE] ;
    int   lines ;

    if (!(file = fopen(filename, "r")))
    {
       fprintf (stderr, "%s\n", "Cannot open file") ; 
       exit (EXIT_FAILURE) ;
    }

    lines = 0 ;
    while (fgets(buff, BUFF_SIZE, file))
    {
        ++lines ;
    }
    fclose (file) ;

    return lines ;
} /* get_file_lines */

/**
 *  \fn init_vars
 */
void init_vars (int processes) 
{
    int  seg_id ;

    seg_id = shmget (
                gen_key(), 
                sizeof(int) * WORDS_LEN, 
                IPC_CREAT | 0660
    ) ;
    
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

    for (int i = 0; i <WORDS_LEN; ++i)
    {
        shared_rep[i] = 0 ;
    }

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Shared memory initialized")) ;

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Creating semaphores")) ;
    sem_create (&sem_id, processes) ;
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Initializing semaphores")) ;
    sem_init (sem_id, processes - 1, SEM_TOKEN_NB) ;
} /* init_vars */

/**
 * \fn print_usage
 */
void print_usage (int count)
{
    fprintf (
        stderr, 
        "%s \n(given: %d)\n", 
        "usage: ./prog <source> <destination> <processes>",
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
    if (atoi(argv[3]) > MAX_FORKS) 
    {
        abort_prog ("Too many subprocesses", EXIT_FAILURE) ;
    }

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Initializing Segment...")) ;
    init_vars(atoi(argv[3])) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Operating forks...")) ;
    create_child ((char*)argv[1], (char*)argv[2], atoi(argv[3])) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Deleting Segment...")) ;
    shmdt(shared_rep) ;

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Deleting Semaphores...")) ;
    sem_destroy(SEM_ID) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Exciting...")) ;
    return 0 ;
} /* main */
