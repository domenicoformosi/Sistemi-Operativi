#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define INPUTERROR -1
int fd[2];

void mkcommand(char *[],char [], char []);
void divisibile(int);
void nonDivisibile(int);
void waitMySonsDeath();
int main (int argc, char *argv[]){

 
/*
    prima controllo tutti gli input
*/
if (argc<4)
{
    printf("Numero di parametri in input illegale! \n");
    exit(INPUTERROR);
}
char file [strlen(argv[1])];
char parola [strlen(argv[2])];
int M = atoi(argv[3]),pid,pid2,nlines;

strcpy(file,argv[1]);
strcpy(parola,argv[2]);

if ( access(file,F_OK) < 0){
    printf("Impossibile aprire il file!\n");
    exit(INPUTERROR);
}

if ( M <= 0){
    printf("Numero illagel!\n");
    exit(INPUTERROR);
}

pipe(fd);
pid = fork();
printf("aaa \n");
if (pid == 0){

    // definisco comporamento di P1
    // grep -c parola file
    close(fd[0]);
    

    close(1);
    dup(fd[1]);
    close(fd[1]);
   
    execl("grep","grep -c",argv[2],argv[3],NULL);
    
}

pid = fork();

if (pid == 0){
    signal(SIGUSR1,divisibile);
    signal(SIGUSR2,nonDivisibile);
    pause();
}

if (pid > 0){
    close(fd[1]);

    read(fd[0],&nlines,sizeof(int));
    printf("%i",nlines);
    if ((nlines%M)==0)

        kill(pid,SIGUSR1);

    else
        kill(pid,SIGUSR2);


    waitMySonsDeath();



}






}
void waitMySonsDeath(){
    int pid,status;
    pid = wait(&status);
    if ( WIFEXITED(status)) printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) printf("Terminazione involontaria per segnale %d\n",WTERMSIG(status));

}
void divisibile(int signum){
    int M;
    read(fd[0],&M,sizeof(M));

    printf("Sono P2, il mio pid è: %i. Il numero di occorrenze è divisibile per %i",getpid(),M);
    exit(0);

}
void nonDivisibile(int signum){
    int M;
    read(fd[0],&M,sizeof(M));

    printf("Sono P2, il mio pid è: %i. Il numero di occorrenze non è divisibile per %i",getpid(),M);
    exit(0);

}
/*
void mkcommand(char *command[], char parola[],char file[]){
    strcpy(command,"grep -c ");
    strcat(command,parola);
    strcat(command," ");
    strcat(command,file);
    strcat(command,"\0");
    printf("%s",command);

}*/