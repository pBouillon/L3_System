/**
 * \file main.c
 * \brief TP_5 launcher
 * 
 * \version 0.0.1
 *
 * \author Pierre Bouillon [https://pierrebouillon.tech/]
 */

/**
 * Time output with a 221675 lines text file:
 *      For 1 process
 *          >$ time ./reader huge_extract.txt aaa 1
 *          real	0m0.323s
 *          user	0m0.216s
 *          sys	    0m0.008s
 *      For 2 processses
 *          >$ time ./reader huge_extract.txt aaa 2
 *          real	0m0.483s
 *          user	0m0.212s
 *          sys	    0m0.028s
 *      For 4 processes
 *          >$ time ./reader huge_extract.txt aaa 4
 *          real	0m0.372s
 *          user	0m0.220s
 *          sys  	0m0.004s
 *      For 8 processes
 *          >$ time ./reader huge_extract.txt aaa 8
 *          real	0m0.421s
 *          user	0m0.260s
 *          sys	    0m0.000s
 *      For 100 processes
 *          >$ time ./reader huge_extract.txt aaa 100
 *          real	0m0.488s
 *          user	0m0.208s
 *          sys	    0m0.020s
 *      For 1000 processes
 *          >$ time ./reader huge_extract.txt aaa 1000
 *          real	0m3.034s
 *          user	0m0.240s
 *          sys	    0m0.080s
 *
 * real : wall clock time - time from start to finish of the call
 * user : amount of CPU time spent in the kernel (outside the kernel) 
 *        within the process
 * sys  : amount of CPU time spent in the kernel within the process
 */

// standards headers
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// custom headers
#include "ressources.h"
#include "semaphore.h"

/**
 * to generate debug if compiled with -DDEBUG
 * do nothing if compiled without this flag
 *
 * usage:
 *      DEBUG_PRINT (("%s\n", "This is a debug message")) ;
 */
#ifdef DEBUG
    # define DEBUG_PRINT(x) printf x
#else
    # define DEBUG_PRINT(x) do {} while (0)
#endif

int  sem_id ;     /*!< semaphore id  */
int  seg_id ;     /*!< shared memory id  */
int* shared_rep ; /*!< pointer to the shared memory */

/**
 * \fn    abort_prog
 * \brief aborting function for main.c
 *
 * On exit, destroy the shared memory and the semaphore
 *
 * \param  msg     reason of the exit
 * \param  err_id  exit code to return
 */
void abort_prog (char* msg, int err_id) 
{
    fprintf (stderr, "%s\n", msg) ;

    DEBUG_PRINT (("DEBUG EXIT -- Deleting shm id %d\n", seg_id)) ;
    shmdt (shared_rep) ;
    shmctl(seg_id, IPC_RMID, 0) ;

    DEBUG_PRINT (("DEBUG EXIT -- Deleting sem id %d\n", sem_id)) ;
    sem_destroy(sem_id) ;

    exit (err_id) ;
} /* abort_prog */

/**
 * \fn    gen_key
 * \brief generate the key for the shared memory
 *
 * \return the generated key
 */
key_t gen_key() 
{
    return ftok(KEY_FILE, KEY_ID) ;
} /* gen_key */

/**
 * \fn    create_child
 * \brief create childs
 *
 * Perform `processes` fork.s 
 * Then make each child read a different part of `source_file`
 * Finally prints shared memory content
 * 
 * \param  source_file  path to the file to read
 * \param  output       begin of the name for the save file
 * \param  processes    amount of fork to perform
 */
void create_child (char *source_file, char *output, int processes) 
{
    int   status, ret ;
    int   begin, lines, step ;
    pid_t pid[processes] ;

    begin = 0 ;
    // max lines of the source_file
    lines = get_file_lines(source_file) ;
    // calculating each steps between each process
    step  = (int)lines/processes ;

    for (int i = 0; i < processes ; ++i) 
    {
        // the last process reads the file to the end
        if (i == processes - 1) 
        {
            step = 0 ;
        }

        if ((pid[i] = fork()) < 0) 
        {
            abort_prog ("Unable to perform `fork`.", EXIT_FAILURE) ;
        } 

        else if (pid[i] == 0) 
        {
            char buff_b[4] ; // buffer for begin
            char buff_e[4] ; // buffer for end

            DEBUG_PRINT (("CHILD %d: \n\tbegin=%d step=%d\n", i, begin, step)) ;

            // filling buffers
            snprintf(buff_b, 3, "%d", begin) ;
            snprintf(buff_e, 3, "%d", step) ;

            // calling the reader to read `step` lines from `begin`
            ret = execl (
                "./file_parser", 
                "file_parser", 
                source_file, 
                output,
                buff_b,
                buff_e,
                (char *)NULL
            ) ;

            if (ret != 0) 
            {
                abort_prog ("`execl` failed.", EXIT_FAILURE) ;
            }
        }
        // increasing step for the next process
        begin += step ;
    }

    /* Parent process */
    // prevents remainings semaphore and shared memory on SIGINT
    set_mask() ;

    // waits for all its subprocesses
    for (int i = 0; i < processes; ++i)
    {
        waitpid(pid[i], &status, 0) ;
        if (status != EXIT_SUCCESS) 
        {
            ++ret ;
        }
    }

    if (ret)
    {
        abort_prog ("Child excited with an error", EXIT_FAILURE) ;      
    }

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "FATHER: Childs terminated")) ;

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "FATHER: Receiving ...")) ;
    
    // Format and display results as: X sentence[s] of [0]Y word[s]
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
 * \fn     get_file_lines
 * \brief  get the number of lines for `filename`
 *
 * \param  filename  file to read
 *
 * \return number of lines
 */
int get_file_lines (char* filename)
{
    FILE *file ;
    char  buff[BUFF_SIZE] ;
    int   lines ;

    if (!(file = fopen(filename, "r")))
    {
        abort_prog("Cannot open file", EXIT_FAILURE) ;
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
 * \fn     init_vars
 * \brief  initialize the semaphore and the shared memory
 */
void init_vars () 
{
    // Create shared memory
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
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Getting memory created")) ;

    // Attached shared memory
    shared_rep = (int*) shmat (seg_id, NULL, 0) ;
    if (shared_rep == NULL) 
    {
        abort_prog (
            "Unable to get a pointer on shared memory",
            EXIT_FAILURE
        ) ;
    }

    // Initialized shared memory with 0
    for (int i = 0; i < WORDS_LEN; ++i)
    {
        shared_rep[i] = 0 ;
    }
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Shared memory initialized")) ;

    // Create semaphore
    sem_create (&sem_id, SEM_NB) ;
    if (sem_id < 0)
    { 
        abort_prog ("Unable to create semaphore", EXIT_FAILURE) ;
    }
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Semaphore created")) ;
    
    // Initialize semaphore
    sem_init (sem_id, SEM_NB, SEM_TOKEN_NB) ;
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Initializing semaphore")) ;
} /* init_vars */

/**
 * \fn     print_usage
 * \brief  prints program usage
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
 * \fn     sig_mask
 * \brief  set sigaction on SIGINT
 */
void set_mask ()
{
    struct sigaction sa ;

    sa.sa_handler = sig_handler ;
    sa.sa_flags   = 0 ;
    sigemptyset(&(sa.sa_mask)) ;
    sigaction(SIGINT, &sa, NULL) ;;
} /* set_mask */

/**
 * \fn     sig_handler
 * \brief  catches SIGINT 
 *
 * Avoid remaining semaphore or shared memory
 *
 * \param  signal  [UNUSED] signal catched
 */
void sig_handler (int signal)
{
    (void)signal ;  
    abort_prog ("Execution interrupted", EXIT_FAILURE) ; 
} /* sig_handler */

/**
 * main
 */
int main (int argc, char const *argv[])
{
    int processes ;

    // if an incorrect number of arg is provided, prints usage
    if (argc != ARGS_MAIN) 
    {
        print_usage (argc) ;
    }

    processes = atoi(argv[3]) ;
    // error if processes < 1 or proccesses > 100
    if (1 > processes || processes > MAX_FORKS)
    {
        abort_prog ("Incorrect number of processes", EXIT_FAILURE) ;
    }

    // if too many processes, doing one per line
    if (processes > get_file_lines((char*)argv[1]))
    {
        processes = get_file_lines((char*)argv[1]) ;
    }

    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Initializing Segment...")) ;
    init_vars() ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Operating forks...")) ;
    create_child ((char*)argv[1], (char*)argv[2], processes) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Deleting Segment...")) ;
    shmdt(shared_rep) ;
    
    DEBUG_PRINT(("DEBUG MAIN -- %s\n", "Exciting...")) ;
    abort_prog ("Exciting successfuly", EXIT_SUCCESS) ;

    return 0 ;
} /* main */
