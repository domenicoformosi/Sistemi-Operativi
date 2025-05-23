#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


/*
    codice per capire come funziona ritorno di status e gestione dei segnali:
    in pratica il bro crea un figlio che si gratta le palle per tipo 30 secondi,
    quindi hai tutto il tempo del mondo per aprire il terminale scrivere "ps" per trovare
    i processi in esecuzione e ammazzare il figlio usando "kill - 9 PID"

*/
int main(){
    int pid,status;
    pid = fork();
    if (pid==0){
        printf("Sono il figlio %d \n",pid);
        signal(SIGINT, SIG_IGN);
        execl("/bin/sleep","sleep","30",(char *) 0);
        exit(0);
    }else{
        pid = wait(&status);
        if ( WIFEXITED(status)) printf("Terminazione volontaria di %d con stato %d \n", pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status)) printf("Terminazione involontaria per segnale %d \n",WTERMSIG(status));
    }
}