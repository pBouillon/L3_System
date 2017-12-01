#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "ressources.h"

int save_nb = 0 ;

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
void gen_file_name (char* result, char *base_name)
{
    pid_t pid ;

    ++save_nb ;
    pid = getpid() ;

    sprintf (
        result, 
        "%s_%d_%d.txt", 
        base_name, 
        pid,
        save_nb
    ) ;
} /* gen_file_name */

/**
 *
 */
void read_lines (char *filename, char *save_dest, int begin, int rows)
{
    FILE *file ;

    char  buff[BUFF_SIZE] ;
    char  out[RES_FILE_NAME] ;

    int   chars, words, total_words, checked_lines, i ;
    int   repartition[WORDS_LEN] = {0} ;


    if (!(file = fopen(filename, "r")))
    {
       fprintf (stderr, "%s\n", "Cannot open file") ; 
       exit (EXIT_FAILURE) ;
    }

    chars = 0 ;
    words = 0 ;
    total_words = 0 ;
    checked_lines = 0 ;

    i = 0 ;
    if (rows == 0) 
    {
        rows = get_file_lines(filename) ;
    }
    while (fgets(buff, BUFF_SIZE, file) 
        && checked_lines < rows)
    {
        if (i < begin)
        {
            ++i ;
            continue ;
        }

        for (i = 0; i < strlen(buff); ++i) 
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
            gen_file_name(out, save_dest), 
            write_file (
                out,
                words, 
                repartition
            ) ;
        }
    }
    
    gen_file_name(out, save_dest) ;
    write_file (
        out,
        words, 
        repartition
    ) ;
    printf("\n%s %s\n", "Last file generated:", out) ;
    printf("\t[Words checked: %d]\n", total_words) ;
    printf("\t[%d lines on %d]\n\n", checked_lines, get_file_lines(filename)) ;
    fclose (file) ; 
} /* read_lines */

/**
 *
 */
void write_file (char *filename, long int words, int count[]) 
{
    int   i ;
    FILE *file ;

    if (!(file = fopen(filename, "w")))
    {
       fprintf(stderr, "%s\n", "Cannot open save file") ; 
       exit (EXIT_FAILURE) ;
    }

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
int main(int argc, char const *argv[])
{
    int   begin, rows;
    char *source, *dest_name ;

    if (argc != ARGS_NUM) 
    {
        print_usage (argc) ;
    }

    
    source    = (char*)argv[1] ;
    dest_name = (char*)argv[2] ;
    begin  = atoi(argv[3]) ;
    rows   = atoi(argv[4]) ;

    read_lines (
        source, 
        dest_name, 
        begin, 
        rows
    ) ;

    return 0 ;
} /* main */
