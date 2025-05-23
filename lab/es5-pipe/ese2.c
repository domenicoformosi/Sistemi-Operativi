#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 4
#define MIN 1
// x generare numero random tra 0 4

typedef struct{
    char cA; //carattere letto da Fa
    char cB; //carattere letto da Fb
    int pos; //posizione
}elemento;

int getRandomValue();

int main(int argc, char * argv[]){

int fd[2],fa,fb,pid;

pipe(fd); 
pid = fork();

if (pid == 0){
    close(fd[1]); // chiudo lato di scrittura che non mi serve

    close(0);//chiudo stdin_
    dup(fd[0]); // ridirigo stdin su lato di lettura della pipe
    close(fd[0]); // chiudo copia del lato di scrittura

    execlp("wc","wc","-l",(char *)0);
    /*
         dentro la exec ci sarà una read da stdin ( che è fd[0] ), quindi quando chiudo
         fd[1] effettivamente quella read appena si svuota la pipe restituirà 0,

    
    */

    exit(0);

}else if (pid>0){
    int rnd,pos=0,tmp,tmp1;
    char ca,cb;
    elemento el;

    if ((fa = open(argv[1],O_RDONLY))<0) { perror("Problemo! \n"); exit(-1);}
    if ((fb = open(argv[2],O_RDONLY))<0) { perror("Problemo! \n"); exit(-1);}
    do{
    rnd = getRandomValue();
    pos += rnd;
    lseek(fa,rnd,SEEK_CUR);
    lseek(fb,rnd,SEEK_CUR);
    tmp=read(fa,&ca,sizeof(char));
    tmp1=read(fb,&cb,sizeof(char));
    if (ca != cb){
        el.cA = ca;
        el.cB = cb;
        el.pos = pos;
        char msg[50];
        sprintf(msg,"Trovata differenza in posizione: %d cA = %c cB = %c\n",el.pos,el.cA,el.cB);
        printf("%s",msg);
        write(fd[1],msg,sizeof(msg));
        /*
            fino a qui tutto okay, invio sta roba a P1, adesso devo gestire
            ridirezione nel figlio
        */
    }
    }while (tmp>0 && tmp1>0);

    close(fd[1]); // mi serve per far finire la exec
     /*
     quando sono arrivato alla fine di almeno uno dei due file, chiudo lato di
     scrittura --> ciò implica che la prossima tentata lettura (read), ritornerà
     0
    */
    close(fa);
    close(fb);
    int status;
    pid = wait(&status);
    if (WIFEXITED(status)) 
        printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Terminazione involontaria per segnale%d\n", WTERMSIG(status));
    }else if (pid<0)
    printf("Errore in fork! \n");
   exit(0);

}







int getRandomValue(){

    return (rand() % MAX)+MIN;
}
