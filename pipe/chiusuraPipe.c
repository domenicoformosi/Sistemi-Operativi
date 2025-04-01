/*
    nel file comunicazioneStupidaWPipe abbiamo visto come un processo puo chiudere un estremo
    della pipe che non usa, diremo che un estremo è effettivamente chiuso se tutti i processi
    che avevano visibilità della pipe chiudono quell'estremo, analizziamo i due casi

    Processo P
    - prova a leggere una pipe vuota con estremo di scrittura bloccato ==>
      la read mi ritorna (<0)
    - prova a scrivere una pipe il cui lato di lettura è effettivamente chiuso ==>
      la write mi restituisce -1 e mi viene inviato un SIGPIPE a P (broken pipe)

    questa roba è effettivamente comoda se ci pensi (pensaci)

*/

/*
    lo scopo di questo esempio è come quello del file precedente solo che adesso il numero
    di messaggi da scambiarsi non è noto a priori ma dovrà finire dopo N secondi,
    è quindi chiaro che useremo un alarm e sfrutteremo il meccanismo di chiusura della pipe
*/

#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define DIM 15

int fd[2];
void fine(int);
void timeout(int);

int main(int argc, char **argv){
    int pid, N;
    char messaggio[DIM]="scaravilli\n";
    if (argc != 2){ printf("Cosa fai?\n"); exit(1);}
    N = atoi(argv[1]);
    pipe(fd);
    pid = fork();

    if (pid == 0){
        signal(SIGPIPE,fine); 
        /*
            l'esercizio sta tutto qua!!! ridefinisco il comportamento della sigpipe
            sapendo che me ne arriverà una quando provero a scrivere sulla pipe chiusa 
            in lettura
        */
       close(fd[0]); // chiudo in lettura che non mi serve
       for (;;) write (fd[1],messaggio,DIM);

    }else if (pid > 0){
        signal(SIGALRM,timeout); // mi ridefinisco comportamento per fine timer

        close(fd[1]); // vabbe hai capito chiudo in scrittura che tanto non mi serve 
        alarm(N);
        for(;;){
            read(fd[0],&messaggio,DIM);
            write(1,messaggio,DIM);
        }

    }
}

void fine(int signum){
    close(fd[1]);
    exit(0);
    /*
        qui essenzialemente chiudiamo effettivamente il lato in scrittura e poi ci uccidiamo :)
    */
   
}
void timeout(int signum){
    int status;
    close(fd[0]);
    /*
        altro pezzo fondamentale --> spieghiamo cosa succede
        in pratica quando finisce il "timer" il processo padre va a chiudere
        il suo lato di lettura, adesso quindi è effettivamente chiuso e un tentativo
        di scrittura provocherà il lancio di SIGPIPE il quale comportamento è definito
        in fine
    */
   wait(&status);
   /*
        si noti che senza la cose questa wait aspetta all'infinito dato che la exit del figlio
        è fatta in fine
   */
  if ( WIFEXITED(status)) printf("Terminazione volontaria con stato %d\n", WEXITSTATUS(status));
  else if (WIFSIGNALED(status)) printf("Terminazione involontaria per segnale %d\n",WTERMSIG(status));
  exit(0);
}

/*
    piu andiamo avanti piu si puo apprezzare come viene "semplificata" la comunicazione tra 
    processi (grazie IPC!!!!!)
*/

