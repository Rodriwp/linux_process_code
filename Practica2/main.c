#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define MAX_NUM_PIDS 10
#define PASS_FILE_PATH "./passfile"
#define BACKUP_PASS_FILE_PATH "./backup_passfile"
#define PASS_FILE_NAME "passfile"
#define PASSLINE 10

/*Signals*/
void backupend_handler(){
    printf("\n\n CHILD:The BackUp is finished. Time for me to died father\n\n");
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
    //PIPE variables
    int fd[2];
    int n_read;
    char buffer[PASSLINE];
    //Control variables
    int daemon = 1;
    int selec = 0;
    //Case 3 variables
    pid_t temppid = 0;
    pid_t child_pids[MAX_NUM_PIDS];
    //Signals
    if (signal(SIGUSR1, backupend_handler) == SIG_ERR) {
       printf(" Impossible catching SIGUSR1\n");
    }
    //Loop
    while(daemon){
        print_menu();
        if(scanf("%d",&selec)!= 1){
            selec = 0;
        }
        switch(selec){
            case 1:
                temppid = fork();
                if (temppid < 0){ /* error occurred */
                    perror("Fork Failed");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0) { /* edit.p */
                    int ret= execlp("/usr/bin/gedit","gedit",PASS_FILE_NAME,NULL); //FIXME: ruta gedit
                    if (ret == -1) {
                        perror("execl: gedit");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 2:
                if (pipe(fd) < 0) {
                    printf("Error in pipe creation.\n");
                }
                temppid = fork();
                if (temppid < 0){ /* error occurred */
                    perror("Fork Failed");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0) { /* pfile */
                    close(fd[1]);
                    FILE* passfile = fopen (BACKUP_PASS_FILE_PATH, "w");
                    while((n_read = read(fd[0], buffer, PASSLINE))>0){
                        fwrite(buffer, sizeof(char),n_read, passfile);
                    }
                    close(fd[0]);
                    fclose(passfile);
                    kill(getppid(),SIGUSR1);
                    exit(EXIT_SUCCESS);
                }
                else{ /*main */
                    close(fd[0]);
                    FILE* passfile = fopen (PASS_FILE_PATH, "r");
                    while(!feof(passfile)){
                        n_read = fread(buffer,sizeof(char),PASSLINE, passfile);
                        write(fd[1], buffer, n_read);
                    }
                    close(fd[1]);
                    fclose(passfile);
                    printf("\nFATHER:End sending the passfile to my child, I'm operative again\n");
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
    printf("Just wait a second. We're doing safe exit\n");
    //TODO: salida segura del programa
    printf("It's done, we always code safe\n");
    //TODO: safe exit
}
