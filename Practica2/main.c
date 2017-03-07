#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#define MAX_NUM_PIDS 10
#define PASS_FILE_PATH "./passfile"
#define BACKUP_PASS_FILE_PATH "./.passwords.backup"
#define PASS_FILE_NAME "passfile"
#define PASSLINE 10
#define MAX_FILEPATH_SIZE 200
#define NUM_THREADS 2
#define PWDS_LENGTH 10

/*Signals*/
void backupend_handler(){
    printf("\n\n CHILD:The BackUp is finished. Time for me to died father.\n\n");
}
/* cASE 4*/
/* PRINTING FUNCTIONS*/
void print_menu(){
    printf("\n******************Manager application********************\n\
           \n\
           Operations:\n\
           \n\
           [1] Edit pass file\n\
           [2] Backup copy\n\
           [3] Automatic time display (CNTRL-C for CET/UTC display change)\n\
           [4] PPT: Parrallel Password Treatment\n\
           [5] Ordered shutdown\n\
           \n\
           \t\tSelect option:");
}

/*Funcion del Apartado 4*/
void *edit_pwd(void *t){
    t = "";
    //pthread_exit(NULL);
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

    //PIPE variables
    int fd[2];
    int n_read;
    char buffer[PASSLINE];
    //Control variables
    int daemon = 1;
    int selec = 0;
    char filepath[MAX_FILEPATH_SIZE];
    //Case 3 variables
    pid_t temppid = 0;
    pid_t child_pids[MAX_NUM_PIDS];
    int child_num = 0;
    //Case 4 PPT
    char (claves[PASSLINE])[PWDS_LENGTH];
    pthread_t threads [NUM_THREADS];
    int t, rc;

    //Signals
    if (signal(SIGUSR1, backupend_handler) == SIG_ERR) {
       printf(" Impossible catching SIGUSR1\n");
    }
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) {
       printf(" Impossible to ignore SIGINT\n");
    }
    //Loop
    while(daemon){
        print_menu();
        if(scanf("%i",&selec)!= 1){
            selec = 0;
        }
        switch(selec){
            case 1:
                temppid = fork();
                if (temppid < 0){ /* error occurred */
                    perror("Fork failed.");
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
                printf("Give me the passfile: ");
                if(scanf("%s",filepath)!= 1){
                    printf("We need the path to the file. Try again\n");
                    break;
                }
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
                    FILE* passfile = fopen (filepath, "r");
                    if(passfile ==  NULL){
                        printf("Error 404: File Not Found\n");
                        kill(temppid,SIGKILL);
                        close(fd[1]);
                    }
                    else{
                        while(!feof(passfile)){
                            n_read = fread(buffer,sizeof(char),PASSLINE, passfile);
                            write(fd[1], buffer, n_read);
                        }
                        close(fd[1]);
                        fclose(passfile);
                        printf("\nFATHER:End sending the passfile to my child, I'm operative again.\n");
                    }
                }
                break;
            case 3:
                if(child_num < MAX_NUM_PIDS){
                    temppid = fork();
                }
                else{
                    printf("You have already enought time daemons.\nIf you need more, please contact with our sales deparment\n");
                    break;
                }
                if (temppid < 0){/*error ocurred*/
                    perror("Fork failed.");
                    exit(EXIT_FAILURE);
                }
                else if (temppid == 0){ /*date*/
                    if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
                       printf("Impossible to ignore SIGUSR1\n");
                    }
                    
                    if (signal(SIGINT, SIGINT_handler) == SIG_ERR){
                        fprintf(stderr,"Cannot handle SIG_INT.\n");
                        exit(EXIT_FAILURE);
                    }
                    
                }
                else{
                    child_pids[child_num]= temppid;
                    child_num++;
                }
                break;
            case 4:
                //TODO: caso hebras
                if(child_num < MAX_NUM_PIDS){
                    temppid = fork();
                }
                else{
                    printf("You have already enought time daemons.\nIf you need more, please contact with our sales deparment\n");
                    break;
                }
                if(temppid < 0){//error ocurred
                    perror("Fork failed.");
                    exit(EXIT_FAILURE);
                }
                else if(temppid == 0){//proceso hijo
                    
                    
                    FILE* stdin;
                    FILE* passfile;
                    
                    fgets (filepath, MAX_FILEPATH_SIZE, stdin);
                    printf("Introduce the name of the passfile to make it secure.\n");
                    printf("hododo");
                    
                    if(strcmp(filepath, PASS_FILE_NAME) == 0)
                        passfile = fopen(filepath, "r+");
                    else
                        printf("El fichero no existe. Inténtelo de nuevo");
                    
                    /*Lectura del fichero de contrasenias*/
                    int i;
                    for(i=0; i< PASSLINE; i++){
                        fgets(claves[i],PWDS_LENGTH,passfile);
                    }
                    fclose(passfile);
                    printf(claves[0]);
                    
                    
                    for (t = 0; t < NUM_THREADS; t++){//Threads creation
                        rc = pthread_create(&threads[t],NULL,edit_pwd,t);
                        if(rc){//error ocurred
//                         //perror("Thread creation failed.");
//                         //exit(EXIT_FAILURE);
//                         }
                        }
                    }
                }
                else{//main
                    
                }
                //pthread_exit(NULL);
                break;
            case 5:
                daemon = 0;
                printf("Thanks for using our manager app. See you soon!\n");
                break;
            default:
                printf("\nInvalid Option. Please try again.\n");
                break;
        }
    }
    printf("Just wait a second. We're doing safe exit.\n");
    int i = 0;
    int status = 0;
    for(i=0; i< child_num; i++){
        kill(child_pids[i], SIGKILL);
    }
    for(i=0; i< child_num; i++){
        waitpid(child_pids[i],&status,0);
    }
    printf("It's done. We always code safe.\n");
    exit(EXIT_SUCCESS);
}