/**
 * \file file_parser.c
 * \brief TP_5 launcher
 * 
 * \version 0.0.1
 *
 * \author Pierre Bouillon [https://pierrebouillon.tech/]
 */

// standards headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
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

int  save_nb = 0 ; /*!< used for save's names and keep the count */
int* shared_rep  ; /*!< pointer to the shared memory with `main.c` */

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
 * \fn     gen_file_name
 * \brief  generate the name of the file for the save
 *
 * Name is <base_name>_<pid>_<save_id|final>.txt
 * _final if this is the last file saved
 *
 * \param  result     buffer to fill with the name generated
 * \param  base_name  begin of the name
 * \param  is_final   1 if this is the last file; 0 otherwise
 */
void gen_file_name (char* result, char *base_name, int is_final)
{
    pid_t pid ;

    ++save_nb ;
    pid = getpid() ;

    DEBUG_PRINT(("DEBUG FILE -- %s\n", "Generating saving file name")) ;
    if(!is_final) 
    {
        sprintf (
            result, 
            "%s_%d_%d.txt", 
            base_name, 
            pid,
            save_nb
        ) ;
    } 
    else {
        sprintf (
            result, 
            "%s_%d_final.txt", 
            base_name, 
            pid
        ) ;
    }
} /* gen_file_name */

/**
 * \fn      get_file_lines
 * \brief   get the total lines of the file `filename`
 *
 * \return  total lines numer
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
 * \fn     get_shm_pointer
 * \brief  get a pointer to the shared memory
 */
void get_shm_pointer() 
{
    int seg_id ;
    
    seg_id = shmget (gen_key(), sizeof(int) * WORDS_LEN, 0) ;
    if (seg_id < 0) 
    {
        fprintf(stderr, "%s\n", "Failed to create a shared memory space") ;
        exit(EXIT_FAILURE) ;
    }
    DEBUG_PRINT(("DEBUG FILE -- %s\n", "Shared memory received")) ;

    shared_rep = (int*) shmat (seg_id, NULL, 0) ;
    if (shared_rep == NULL) 
    {
        fprintf(stderr, "%s\n", "Unable to get a pointer on shared memory") ;
        exit(EXIT_FAILURE) ;
    }
    *shared_rep = 0 ;
    DEBUG_PRINT(("DEBUG FILE -- %s\n", "Shared memory initialized")) ;
} /* get_shm_pointer */

/**
 * \fn     print_usage
 * \brief  prints program usage
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
 * \fn     read_lines
 * \brief  from `filename` read `rows` lines from `begin` 
 *
 * perform a save every `SAVE_LIMIT` rows
 *
 * \param  filename   file to read
 * \param  save_dest  base name for the save
 * \param  begin      line to start reading
 */
void read_lines (char *filename, char *save_dest, int begin, int rows)
{
    FILE *file ;

    char buff[BUFF_SIZE] ;
    char out[RES_FILE_NAME] ;

    int  chars, words, total_words, checked_lines, count;
    int  repartition[WORDS_LEN] = {0} ;
    int  sem_id ;


    if (!(file = fopen(filename, "r")))
    {
       fprintf (stderr, "%s\n", "Cannot open file") ; 
       exit (EXIT_FAILURE) ;
    }

    chars = 0 ;
    words = 0 ;
    total_words = 0 ;
    checked_lines = 0 ;

    if (rows == 0) 
    {
        rows = get_file_lines(filename) ;
    }

    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(),"Begin reading")) ;
    count = 0 ;
    while (fgets(buff, BUFF_SIZE, file) && checked_lines < rows)
    {
        if (count < begin)
        {
            ++count ;
            continue ;
        }

        for (int i = 0; i < (int)strlen(buff); ++i) 
        {
            if (strchr(SEPARATORS, buff[i]) != NULL)
            {
                if (chars != 0)
                {
                    chars = 0 ;
                    ++words ;
                }
            }
            else
            {
                ++chars ;
            }
        }

        if (chars != 0)
        {
            ++words ;
        }
        if (words <= WORDS_LEN - 1)
        {
            ++repartition[words] ;
        }
        else
        {
            ++repartition[WORDS_LEN - 1] ;
        }

        total_words += words ;
        ++checked_lines ;
        chars = 0 ;
        words = 0 ;

        if (++total_words % SAVE_LIMIT == 0)
        {
            DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Saving part of result")) ;
            gen_file_name(out, save_dest, 0), 
            write_file (
                out,
                words, 
                repartition
            ) ;
        }

    }
    
    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Final save")) ;

    gen_file_name(out, save_dest, 1) ;
    write_file (
        out,
        words, 
        repartition
    ) ;

    // Uncomment to see sub process progess
    /*
    printf("\n%s %s\n", "Last file generated:", out) ;
    printf("\t[Words checked: %d]\n", total_words) ;
    printf("\t[%d lines on %d]\n", checked_lines, get_file_lines(filename)) ;
    */

    if (sem_list(&sem_id, SEM_NB))
    {
        fprintf(stderr, "%s\n", "Unable to get semaphore.") ;
    }

    P (sem_id, SEM_NB) ;
        DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Synchronizing ...")) ;    
        for (int i = 0; i < WORDS_LEN; ++i)
        {
            shared_rep[i] += repartition[i] ;
        }
    V (sem_id, SEM_NB) ; 
    shmdt(shared_rep) ;
    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Synchronization success")) ;

    fclose (file) ; 
} /* read_lines */

/**
 * \fn     write_file
 * \brief  perform the save
 *
 * write each `count` in `filename`
 *
 * \param  filename  file to write
 * \param  words     [UNUSED] words read
 * \param  count[]   words read for each line size
 */
void write_file (char *filename, long int words, int count[]) 
{
    int   i ;
    FILE *file ;

    (void)words ;

    if (!(file = fopen(filename, "w")))
    {
       fprintf(stderr, "%s\n", "Cannot open save file") ; 
       exit (EXIT_FAILURE) ;
    }

    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Saving ...")) ;
    fprintf(file, "%s\n", "Words checked: ") ;
    for (i = 0; i < WORDS_LEN; ++i)
    {
        fprintf(file, "\t%d : %d\n", i, count[i]) ;
    }

    fclose (file) ;
} /* write_file */

/**
 * main
 */
int main (int argc, char const *argv[])
{
    int   begin, rows ;
    char *source, *dest_name ;

    if (argc != ARGS_READER) 
    {
        print_usage (argc) ;
    }

    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Getting pointer")) ;
    get_shm_pointer() ;

    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Getting args")) ;    
    source    = (char*)argv[1] ;
    dest_name = (char*)argv[2] ;

    begin  = atoi(argv[3]) ;
    rows   = atoi(argv[4]) ;

    DEBUG_PRINT(("DEBUG %d -- \n\tstart: %d | end: %d\n", getpid(), begin, rows)) ;

    DEBUG_PRINT(("DEBUG %d -- %s\n", getpid(), "Start reading")) ;
    read_lines (
        source, 
        dest_name, 
        begin, 
        rows
    ) ;

    return 0 ;
} /* main */
