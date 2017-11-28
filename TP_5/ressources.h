#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define ARGS_NUM      5
#define BUFF_SIZE     1024
#define RES_FILE_NAME 100
#define SEPARATORS    " ,;:.!?\n"
#define WORDS_LEN     30

#define TEMPO         1

void print_usage () ;
void read_lines (char *filename, int begin, int rows) ;
char *get_file_name (char* base_name) ;
void read_lines (char *filename, int begin, int rows) ;
void write_file (char *filename, long int words, int count[]) ;

#endif
