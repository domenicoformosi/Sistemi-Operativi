#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
typedef struct{
    char cA; //carattere letto da Fa
    char cB; //carattere letto da Fb
    int pos; //posizione
}elemento;


void deleteTmp(int);
void printTmp(int);
int getRandomValue();

int main(int argc, char * argv[]){
srand(time(NULL));

int pid1,pid2,fa,fd,fb,rnd,pos=0,tmp,tmp1;
char ca,cb;
elemento el;



pid1 = fork();

if (pid1==0){
    //figlio 1: ridefinisco il comportamendo per SIGUSR1 e mi metto in wait
    signal(SIGUSR1,deleteTmp);
    printf("P1: settato nuovo comportamento,aspetto!\n");
    pause();

    printf("Arrivato signal! File cancellato\n");



    exit(0);
}

pid2=fork();

if (pid2==0){
    //figlio 2: idem qua
    signal(SIGUSR1,printTmp);
    printf("P2: settato nuovo comportamento,aspetto!\n");
    pause();

    printf("Arrivato signal! avviso P1\n");

    kill(pid1,SIGUSR1); 
    exit(0);
}

if ((fa = open(argv[1],O_RDONLY))<0) { perror("Problemo! \n"); exit(-1);}
if ((fb = open(argv[2],O_RDONLY))<0) { perror("Problemo! \n"); exit(-1);}
fd = creat("Fdiff",0640);
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
        printf("rnd = %d pos = %d letto: %c - %c\n",rnd,el.pos,el.cA,el.cB);
        write(fd,&el,sizeof(elemento));
    }
    }while (tmp>0 && tmp1>0);

    printf("invio segnale a P2!\n");
    kill(pid2,SIGUSR1);

}


int getRandomValue(){

    return (rand() % 4)+1;
}

void printTmp(int signum){
    int fd,nread;
    printf("Sono nella printTmp \n");
    if (fd=open("Fdiff",O_RDONLY)<0){
        perror("Problemo!\n");
        exit(-1);
    }
    elemento el;
    while ( (read(fd,&el,sizeof(el))) >0){
        printf("P2: Trovata una differenza in posizione pos: %d cA = %c cB = %c\n",
        el.pos,el.cA,el.cB);
    }
    
}

void deleteTmp (int signum){
    if (unlink("Fdiff")<0) {perror("Problemo cancellazione! \n"); exit(-1);}
}



