#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define  ARGS_READER     5   /*!< args required for the file_parser */
#define  ARGS_MAIN       4   /*!< args required for the main  */
#define  MAX_FORKS       100 /*!< fork limit */

#define  KEY_FILE        "./file_parser.c" /*!< file used for ftok */
#define  KEY_ID          42                /*!< key for ftok*/

#define  BUFF_SIZE       1024 /*!< Buffer size for reading */
#define  RES_FILE_NAME   100  /*!< Buffer for the output file name */

#define  SEPARATORS      " ,;:.!?\n" /*<! Separators when reading */
#define  SAVE_LIMIT      10000       /*<! Lines read before save */

#define  WORDS_LEN       30   /*<! Max words per lines */

#define  SHM_ID          1337 /*<! Shared memory ID */
#define  NO_OPT          "0"  /*<! nopt for execl (ex part 2.1) */

#define  SEM_ID          0    /*<! semaphore ID */
#define  SEM_NB          1    /*<! semaphore number */
#define  SEM_TOKEN_NB    1    /*<! semaphore token amount*/

int  get_file_lines (char* filename) ;

key_t gen_key() ;

void gen_file_name (char *result, char *base_name, int is_final) ;

void print_usage (int count) ;

void read_lines (
    char *filename, 
    char *save_dest, 
    int begin, 
    int rows
) ;

void sig_handler (int signal) ;

void set_mask () ;

void write_file (
    char *filename, 
    long int words, 
    int count[]
) ;

#endif /* RESSOURCES_H_ */
