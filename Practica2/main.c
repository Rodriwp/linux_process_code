#define UTC 0
#define CET 1
void print_menu(){
    printf("\n****Manager application****\n\
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
    int date_format = UTC;
    while(daemon){
        print_menu();
        //TODO: pick number from console
        switch(selec){
            case 1:
                //TODO: gedit
                break;
            case 2:
                //TODO: backup passwords
                break;
            case 3:
                //TODO:TIME
                break;
            case 4:
                daemon = 0;
                printf("Thanks for using our manager app. See you soon");
                break;
            default:
                printf("\nInvalid Option. Please try again\n");
                break;
        }

    }
    //TODO: safe exit
}
