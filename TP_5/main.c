#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ressources.h"


/**
 * \fn void create_child ()
 * \brief create one child
 */
void create_child (char *source_file, char *output) 
{
    int   status ;
    pid_t pid ;

    if ((pid = fork()) < 0) 
    {
        fprintf(stderr, "%s\n", "Unable to perform `fork`.");
        exit(EXIT_FAILURE) ;
    }
    // Child process
    else if (pid == 0) 
    {
        int ret ;
        ret = execl(
                "./file_parser", 
                "file_parser", 
                source_file, 
                output,
                NO_OPT,
                NO_OPT,
                (char *)NULL
            ) ;
        if (ret != 0)
        {
            fprintf(stderr, "%s\n", "`execl` failed.");
            exit(EXIT_FAILURE) ;
        }
        exit(EXIT_SUCCESS) ;
    }
    // Parent process
    else 
    {
        waitpid(pid, &status, 0) ;
        exit(EXIT_SUCCESS) ;
    }
}

int main(int argc, char const *argv[])
{
    create_child ((char*)argv[1], (char*)argv[2 ]) ;
    return 0;
}
