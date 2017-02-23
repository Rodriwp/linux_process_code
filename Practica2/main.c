#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

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

static void SIGINT_handler (int signo){
    if (signo == SIGINT){
        static int change = 0;
        
        if (change % 2 == 0)
            system("date\n");
        else
            system("date -u\n");
        change++;
    }
    else{
	fprintf(stderr, "Se ha producido un error al manejar la señal.\n");
        exit(EXIT_FAILURE);
    }
}

void main(){
    signal(SIGINT, SIG_IGN);
    
    int daemon = 1;
    int selec = 0;
    int temppid = 0;

    while(daemon){
        print_menu();
        if(scanf("%d",&selec)!= 1){
        }
        switch(selec){
            case 1:
                temppid = fork();
                if (temppid < 0){ /* error occurred */
                    perror("Fork failed.");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0) { /* edit.p */
                    int ret= execlp("/usr/bin/gedit","gedit","pass_file","&",NULL); //FIXME: ruta gedit
                    if (ret == -1) {
                    perror("execl: gedit");
                    }
                    exit(EXIT_FAILURE);
                }
                else { /* main process */
                    //TODO: gestionar el hijo
                }
                break;
            case 2:
                //TODO: backup passwords
                break;
            case 3:
                //TODO:TIME
                temppid = fork();
                if (temppid < 0){/*error ocurred*/
                    perror("Fork failed.");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0){ /*date*/
                    signal(SIGUSR1, SIG_IGN);
                    
                    if (signal(SIGINT, SIGINT_handler) == SIG_ERR){
                        fprintf(stderr,"Cannot handle SIG_INT.\n");
                        exit(EXIT_FAILURE);
                    }
                    while(1);
                }
                break;
            case 4:
                daemon = 0;
                printf("Thanks for using our manager app. See you soon!\n");
                break;
            default:
                printf("\nInvalid Option. Please try again.\n");
                break;
        }
    }
    //TODO: safe exit
}
