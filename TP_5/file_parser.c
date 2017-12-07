#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "ressources.h"
#include "semaphore.h"

#ifdef DEBUG
    # define DEBUG_PRINT(x) printf x
#else
    # define DEBUG_PRINT(x) do {} while (0)
#endif


int  save_nb = 0 ;
int* shared_rep ;

int g_begin, g_end ;

/**
 *
 */
key_t gen_key() 
{
    return ftok(KEY_FILE, KEY_ID) ;
} /* gen_key */

/**
 *
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
 *
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
 *
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

    // Uncomment to see sub process state
    /*
    printf("\n%s %s\n", "Last file generated:", out) ;
    printf("\t[Words checked: %d]\n", total_words) ;
    printf("\t[%d lines on %d]\n", checked_lines, get_file_lines(filename)) ;
    printf("\t[From line %d for %d lines]\n\n", g_begin, g_end) ;
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
 *
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
 *
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

    g_begin = begin ;
    g_end   = rows ;
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
