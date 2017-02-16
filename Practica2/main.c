#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    int temppid = 0;
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
    //TODO: safe exit
}
