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

    /*
        Il processo P1 deve leggere le triplette <cA,cB,pos> inviategli da
        P0
    */

   close(fd[1]); // chiudo estremo di scrittura ( non mi serve )
   elemento el;
   while( read(fd[0],&el,sizeof(elemento)) > 0 ){
    //read mi ritorna 0 se se se lato di scrittura effettucamente chiuso e pipe vuota
    printf("P1: Trovata una differenza in posizione pos: %d cA = %c cB = %c\n",
        el.pos,el.cA,el.cB);
   }
   printf("P1: Lettura terminata!! \n");
   close(fd[1]);
   /* 
    se esco dal while ho teoricamente terminato tutto quello che c'era 
    nella  pipe, mi dovrò occupare nel codice del padre di chiudere
    lato di scrittura quando termino di leggere il file (altrimenti
    read non mi ritorna 0 (penso))
    */
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
        //printf("rnd = %d pos = %d letto: %c - %c\n",rnd,el.pos,el.cA,el.cB);
        write(fd[1],&el,sizeof(elemento));
    }
    }while (tmp>0 && tmp1>0);

    close(fd[1]); 
    close(fa);
    close(fb);
    /*
     quando sono arrivato alla fine di almeno uno dei due file, chiudo lato di
     scrittura --> ciò implica che la prossima tentata lettura (read), ritornerà
     0
    */
   

}



}



int getRandomValue(){

    return (rand() % MAX)+MIN;
}
