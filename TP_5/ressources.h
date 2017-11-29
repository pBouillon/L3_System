#ifndef RESSOURCES_H_
#define RESSOURCES_H_

#define ARGS_NUM      5
#define BUFF_SIZE     1024
#define RES_FILE_NAME 100
#define SEPARATORS    " ,;:.!?\n"
#define WORDS_LEN     30

int  get_file_lines (char* filename) ;

char *gen_file_name (char* base_name) ;

void print_usage () ;
void read_lines (
	char *filename, 
	char* save_dest, 
	int begin, 
	int rows
) ;
void write_file (
	char *filename, 
	long int words, 
	int count[]
) ;

#endif
