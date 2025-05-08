#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void sr1Handler(int);
void timerHandler(int);
int main(int argc, char * argv[]){

// prima roba da fare: check dell'input
/*
 1 parametro deve essere il nome di file assoluto
 2 C deve essere un carattere
 3 t un intero positivo
*/
printf("a");
if (argc != 4){
    printf("Sintassi!");
    exit(-1);
}
int t = atoi(argv[3]);
if (t<=0){
    printf("Sintassi! t < 0");
    exit(-1);
}

/*
Il programma P0 deve generare due figli P1 e P2 coi seguenti compiti: 
P1 deve contare il numero X di occorrenze del carattere C nel file Fin e 
passare tale valore al processo P2
..considerazioni pt1..
chiaramente per gestire comunicazione tra i processi uso una pipe --> non devo
lavorare con segnali
*/
int fd[2],pid,pid2,fin,counterT=0,counter=0,status;
char C = argv[2][0],ch;
pipe(fd);
signal(SIGUSR1,sr1Handler);
signal(SIGALRM,timerHandler);
alarm(t);
pid = fork();
printf("a");
if (pid == 0) {

    /*
        Sono P1, prima cosa che faccio è chiudere lato di scrittura della pipe, poi 
        apro file, conto numero di occorrenze con exec + wc (?) e scrivo sull pipe
    */
   printf("a");
   sleep(6);
   close(fd[0]);
   if ((fin = open(argv[1],O_RDONLY))<0){ perror("Problema nell'apertura del file!"); exit(-1);}
   while (read(fin,&ch,sizeof(ch))) if (ch==C) counter ++;

   write(fd[1],&counter,sizeof(counter));
   close(fin);
   close(fd[1]);
   exit(0);
}
pid2=fork();

if (pid2==0){
    
    close(fd[1]);
    if ((fin = open(argv[1],O_RDONLY))<0){ perror("Problema nell'apertura del file!"); exit(-1);}
    while (read(fin,&ch,sizeof(ch))) counterT ++;

    read(fd[0],&counter,sizeof(counter));
    if (counter > counterT/2 ) kill(getppid(),SIGUSR1);

}

if (pid > 0){
    pid = wait(&status);
    if ( WIFEXITED(status)) printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) printf("Terminazione involontaria per segnale %d\n",WTERMSIG(status));


}


}

void sr1Handler(int signum){
    printf("Fin contiene più del 50 per cento di caratteri C");
}

void timerHandler(int signum){
    printf("Uccido tutto");
    fflush(stdout);
    kill(0,SIGKILL);
    /*
        La sigkill non dava il tempo di svuotare buffer quindi ho dovuto 
        usare fflush che mi garantisce lo svuotamento
    */
    exit(-1);
}