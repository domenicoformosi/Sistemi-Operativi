#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
    *3.ProcessiUnix* pag.54
    Programma NAIVE per capire +- come funziona shell in foreground, 
    è orribile, (nelle slide è scritto male e non funziona nemmeno),
    l'idea alla base è comunque quella, essenzialemente la creazione
    di un figlio per ogni comando che l'utente scrive, questa versione
    non permette di passare parametri o robe del genero pero dai si 
    capisce comunque. Immagina che tipo la shell (il padre) è un pappone
    e gestisce tutti i figli senza sporcarsi le mani piu di tanto :)
*/
int main(){
    int stato, atteso, pid; char st[80];
    for (;;){
        pid = fork();
        if ((pid < 0 )) {perror("diocane"); exit(1);}

        if (pid == 0){
            printf("Dimmi quello che devo fare e io lo faccio \n");

            scanf("%s", st);

            execlp(st,st,(char *)0);
            perror("error:");
            exit(0);
        }
        else
        {
            atteso = wait(&stato);
            
            printf("Vuoi fare altra roba mio sommo padrone ? Y/N \n");
            scanf ("%s",st);

            if (strcmp(st,"Y") != 0) exit(0);

        }
    }

}