#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define  ARGS_NUM        5
#define  BUFF_SIZE       1024
#define  RES_FILE_NAME   100
#define  SEPARATORS      " ,;:.!?\n"
#define  SAVE_LIMIT      10000
#define  WORDS_LEN       30

#define  NO_OPT          "0"

int  get_file_lines (char* filename) ;

void gen_file_name (char *result, char *base_name) ;

void print_usage (int count) ;

void read_lines (
    char *filename, 
    char *save_dest, 
    int begin, 
    int rows
) ;

void write_file (
    char *filename, 
    long int words, 
    int count[]
) ;

#endif /* RESSOURCES_H_ */
