//Practica 1: Ejercicio numero uno
//@author Rodrigo Mompó Redoli

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

void main(int argc, char *argv[])
{
    pid_t pid = 0;
    pid = fork();
    if (pid < 0){ /* error occurred */
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { /* child_1 process */
	    while (1){
	      printf("A, B, C");
	      //sleep();
	    }
    }
    else { /* parent process */
    /* parent waits for the child to complete */
      pid = fork();
      if (pid < 0){ /* error occurred */
        perror("Fork Failed");
        exit(EXIT_FAILURE);
      }
      else if (pid == 0) {/* child_2 process */
	    while (1){
	      printf("a, b, c");
	      //sleep();
	    }
      }
      while (1){
	printf("El padre esta ejecutando . . .");
	//sleep();
      }
      exit(EXIT_SUCCESS);
    }
}

