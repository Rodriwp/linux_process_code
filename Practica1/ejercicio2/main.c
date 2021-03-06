/*Practica 1: Ejercicio 2
 *
 *@author Rodrigo Mompó Redoli
 *@author Jorge Serrano Dominguez
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>

void main(int argc, char *argv[])
{
    pid_t pid = 0;
    printf("El padre esta ejecutando . . . \n");
    pid = fork();
    if (pid < 0){ /* error occurred */
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { /* child process */
        pid = fork();
        if (pid < 0){ /* error occurred */
            perror("Fork Failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) { /* grandchild process */
            int ret2 = execlp("./fecha","fecha",NULL);
            if (ret2 == -1) {
            perror("execl: fecha");
            }
        }
        else { /* child process */
            wait(NULL);
            int ret = execl("/bin/ls", "ls", NULL);
            if (ret == -1) {
            perror("execl: ls");
            }
        }
    }
    else { /* parent process */
        wait(NULL);
        printf("Mis hijos han terminado, adios\n");
        exit(EXIT_SUCCESS);
    }
}

