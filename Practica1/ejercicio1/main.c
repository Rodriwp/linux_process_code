//Practica 1: Ejercicio numero uno
//@author Rodrigo Mompó Redoli

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main(int argc, char *argv[])
{
    pid_t pid = 0;
    fprint("El padre esta ejecutando . . .");
    pid = fork();
    if (pid < 0){ /* error occurred */
        fprintf(stderr, "Fork Failed");
        exit(-l);
    }
    else if (pid == 0) { /* child process */

    }
    else { /* parent process */

    /* parent waits for the child to complete */
        wait(NULL);
        printf("Child Complete");
        exit(O);
    }
}

