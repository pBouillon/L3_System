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
void print_usage () 
{
    fprintf (
        stderr, 
        "%s\n", 
        "usage: ./prog <source> <filename> <begin> <lines>"
    ) ;
    exit (EXIT_FAILURE) ;
}

/**
 *
 */
int get_file_lines (char* filename)
{
    FILE *file ;
    char *buff[BUFF_SIZE] ;
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
}

/**
 *
 */
char *gen_file_name (char* base_name)
{
    pid_t pid ;
    char res_name[RES_FILE_NAME] ;

    ++save_nb ;
    pid = getpid() ;

    sprintf (
        res_name, 
        "%s_%d_%d.txt", 
        base_name, 
        pid,
        save_nb
    ) ;
    return res_name ;
}

/**
 *
 */
void read_lines (char *filename, char* save_dest, int begin, int rows)
{
    FILE *file ;

    char  buff[BUFF_SIZE] ;
    char  out[RES_FILE_NAME] ;

    int   chars, words, total_words, i ;
    int   repartition[TAILLE_NB_MOT] = {0} ;


    if (!(file = fopen(filename, "r")))
    {
       fprintf (stderr, "%s\n", "Cannot open file") ; 
       exit (EXIT_FAILURE) ;
    }

    chars = 0 ;
    words = 0 ;
    total_words = 0 ;

    while (fgets(buff, BUFF_SIZE, file))
    {
        for (i = 0; i < strlen(buff); ++i) 
        {
            if (strchr(SEPARATOR, buff[i]) != NULL)
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
        chars = 0 ;
        words = 0 ;

        if (++total_words % 10000 == 0)
        {
            write_file (
                gen_file_name(save_dest), 
                words, 
                repartition
            ) ;
        }
    }
    
    write_file (
                (out = gen_file_name(save_dest)), 
                words, 
                repartition
            ) ;
    printf("%s %s\n", "Last file generated:", out) ;
    fclose (file) ; 
}

/**
 *
 */
void write_file (char *filename, long int words, int count[]) 
{
    int   i ;
    FILE *file ;

    output = gen_file_name (filename) ;

    if (!(file = fopen(output, "w")))
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
}

/**
 *
 */
int main(int argc, char const *argv[])
{
    int lines ;

    if (argc != ARGS_NUM) 
    {
        print_usage() ;
    }

    return 0 ;
}
