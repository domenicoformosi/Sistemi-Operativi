#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int fd[2];
int main (int argc, char * argv[]){

    if (argc != 4){
        printf("Numero di parametri in input illegale!\n");
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] == '/' || argv[2][0] == '/'){
        printf("Uno dei due path non è assoluto (magari entrambi)\n");
        exit(EXIT_FAILURE);
    }
    if (open(argv[1],O_RDONLY) <  0){
        printf("Broo %s non esiste!!!\n",argv[1]);
        exit(EXIT_FAILURE);
    }
     if (open(argv[2],O_RDONLY) >= 0){
        printf("Broo %s esiste (non dovrebbe) !\n",argv[2]);
        exit(EXIT_FAILURE);
    }  
    if (strlen(argv[3])>1)  {
        printf("Non mi hai passato il carattere!\n");
        exit(EXIT_FAILURE);
    }
    if (argv[3][0] != 'P' && argv[3][0] != 'D' ) {
        printf("C è diverso da P o D!\n");
        exit(EXIT_FAILURE);
    }
    if (atoi(argv[4])<=0){
        printf("T <= 0 !\n");
        exit(EXIT_FAILURE);
    }

    int pid[2];
    pipe(fd);

    pid[1] = fork();

    if (pid[1] == 0){


    
    }

    pid[2] = fork();

    if (pid[2] == 0){

        close(fd[0]);

        close(1);
        dup(fd[1]);
        close(fd[1]);

        excelp("wc","wc", "-l",argv[1],"|"," awk", "'{print $1}'");
        perror


    }






}