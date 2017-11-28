#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "ressources.h"


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
    exit(EXIT_FAILURE) ;
}

/**
 *
 */
int get_file_lines (char* filename)
{
    FILE *file ;
    char *buff[BUFF_SIZE] ;
    int   lines ;

    if(!(file = fopen(filename, "r")))
    {
       fprintf (stderr, "%s\n", "Cannot open file") ; 
       exit (EXIT_FAILURE) ;
    }

    lines = 0 ;
    while (fgets(buff, BUFF_SIZE, file))
    {
        ++lines ;
    }
    fclose(file) ;

    return lines ;
}

/**
 *
 */
char *get_file_name (char* base_name)
{
    int ts ;
    char res_name[RES_FILE_NAME] ;

    ts = (int)time(NULL) ;
    return sprintf(res_name, "%s_%c.txt", base_name, ts) ;
}

/**
 *
 */
void read_lines (char *filename, int begin, int rows)
{
    FILE *file ;
    char *buff[BUFF_SIZE] ;
    int   chars, words, total_words, i ;
    int   repartition[TAILLE_NB_MOT] = {0};

    if(!(file = fopen(filename, "r")))
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
            ++repartition[WORDS_LEN] ;
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
            write_file (filename, words, repartition) ;
            wait(TEMPO) ;
        }
    }
    
    write_file (filename, words, repartition) ;
    fclose(file) ; 
}

/**
 *
 */
void write_file (char *filename, long int words, int count[]) 
{
    int i ;
    FILE *file ;
    char output[RES_FILE_NAME] ;

    output = get_file_name (filename) ;

    if(!(file = fopen(output, "w")))
    {
       fprintf(stderr, "%s\n", "Cannot open save file") ; 
       exit (EXIT_FAILURE) ;
    }

    fprintf(file, "%s\n", "Words checked: ");
    for (i = 0; i < WORDS_LEN; ++i)
    {
        fprintf(file, "\t%d : %d\n", i, count[i]);
    }

    fclose(file) ;
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

    lines = get_file_lines (argv[1]) ;

    return 0 ;
}
