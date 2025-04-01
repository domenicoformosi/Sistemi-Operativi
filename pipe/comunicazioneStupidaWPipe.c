/* 
    devo imparare a dare nomi piu brevi.. COMUNQUE
    finalmente ci siamo tolti dalle palle la comunicazione tramite file che solo 
    a pensarci mi viene la febbre e possiam introdurre la PIPE, in pratica l'IPC
    ci regala questo strumento in grado di far comunicare processi della stessa
    gerarchia

    QUALCHE INFO SULLE PIPE PRIMA DI VEDERE IL CODICE
    le pipe sono
        -UNIDIREZIONALI: quindi da un lato leggo e da uno scrivo essenzialmente
        -MOLTI-a-MOLTI: più processi possono scrivere e leggere mediante stessa pipe
        -CAPACITà LIMITATA: possiamo accodare un numero limitato di messaggi
    ci troviamo quindi davanti ad uno strumento di comunicazione ASINCRONA, nel senso 
    che io invio un messaggio poi non so se quello a cui lo volevo mandare lo legge,
    cioè non è che ci si mette effettivamente d'accordo, la roba effettivamente figa è che
    possiamo usare le primitive dei file sulle pipe, questo perchè i due estremi della pipe
    vengono trattati come file, infatti avremo due file descriptor
*/
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    int pid;
    char msg[] = "Ciao papà";
    int fd[2]; // due file descriptor della pipe fd[0] x leggere altro per scrivere
    pipe(fd);
    /*
        qui ho usato la system call pipe a cui ho passato il mio array di fd, questa riga
        implica che nella tabella dei file aperti del processo verranno aggiunti due elementi
        pipe mi restituisce 0 se ha successo altrimenti <0 (raro ma vedremo dopo che è utile)

        !!! ultima roba teorica !!! 
        le read e le write sono sospensive, vuol dire che se la pipe è piena aspetto si svuoti per scrivere
        o se la pipe è vuota aspetto venga scritto qualcosa, vedremo successivamente poi questo meccanismo
        associato alla chiusura definitiva di un lato della pipe 
    */

    pid = fork();
    if (pid == 0){
        close(fd[0]); // buona norma se devi scrivere chiudere il canale in lettura
        write(fd[1],msg,10);

    }else{
        close(fd[1]); // idem qua fra
        read(fd[0],&msg,10);
        write(1,msg,10);
    }

}

/*  
    Se non l'hai gia fatto ti invito ad apprezzare il risparmio di tempo rispetto
    alla comunicazione tramite file
*/