#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define  ARGS_NUM        5

#define  KEY_FILE        "./file_parser.c"
#define  KEY_ID          42

#define  BUFF_SIZE       1024
#define  RES_FILE_NAME   100

#define  SEPARATORS      " ,;:.!?\n"
#define  SAVE_LIMIT      10000

#define  WORDS_LEN       30

#define  SHM_ID          1337
#define  NO_OPT          "0"

#define  SEM_ID          0
#define  SEM_NB          1
#define  SEM_TOKEN_NB    1

int  get_file_lines (char* filename) ;

key_t gen_key() ;

void gen_file_name (char *result, char *base_name) ;

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
