#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define MAX_NUM_PIDS 10

/*Signals*/
void backupend_handler(){
    printf("\n\nThe BackUp is finished\n\n");
}

void print_menu(){
    printf("\n******************Manager application********************\n\
           \n\
           Operations:\n\
           \n\
           [1] Edit pass file\n\
           [2] Backup copy\n\
           [3] Automatic time display (CNTRL-C for CET/UTC display change)\n\
           [4] Ordered shutdown\n\
           \n\
           \t\tSelect option:");
}

void main(){
    int daemon = 1;
    int selec = 0;
    pid_t temppid = 0;
    pid_t child_pids[MAX_NUM_PIDS];
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
       printf(" Impossible catching SIGUSR1\n");
    }
    while(daemon){
        print_menu();
        if(scanf("%d",&selec)!= 1){
        }
        switch(selec){
            case 1:
                temppid = fork();
                if (temppid < 0){ /* error occurred */
                    perror("Fork Failed");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0) { /* edit.p */
                    int ret= execlp("/usr/bin/gedit","gedit","pass_file_original",NULL); //FIXME: ruta gedit
                    if (ret == -1) {
                        perror("execl: gedit");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 2:
                //TODO: pipe creation
                temppid = fork();
                if (temppid < 0){ /* error occurred */
                    perror("Fork Failed");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0) { /* main backup */
                        //TODO: main file procces
                }
                else{
                    temppid = fork();
                    if (temppid < 0){ /* error occurred */
                        perror("Fork Failed");
                        exit(EXIT_FAILURE);
                    }
                    else if (temppid == 0) { /* pfile */
                        //TODO: backup code
                    }else{//main
                        //TODO:close all main pipe
                    }
                }
                break;
            case 3:
                //TODO:TIME
                break;
            case 4:
                daemon = 0;
                printf("Thanks for using our manager app. See you soon\n");
                break;
            default:
                printf("\nInvalid Option. Please try again\n");
                break;
        }
    }
    printf("Just wait a second. We're doing safe exit");
    //TODO: salida segura del programa
    printf("It's done, we always code safe");
    //TODO: safe exit
}
