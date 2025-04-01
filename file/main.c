/*

    Una volta che abbiamo tutti i nostri processi sarebbe molto carino se questi fossero 
    effetivamente in grado di comunicare tra di essi, ci sono tanti metodi per fare ciò
    successivamente vedremo come utilizzare pipe e altri metodi fino ad aeree di memoria
    condivise (nonostante unix sia ad ambiente locale), in questo piccolo esempio vediamo
    come implementare una comunicazione mediante l'astrazione dei file

*/


/*
    COSA DOBBIAMO FARE?? 
    il figlio deve prendere il ruolo di mittente dei messaggi e il padre di destinatario,
    il numero di messaggi e la lora dimensione sono assunti prefissati
*/

#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define max 10
#define dim 15

typedef char msg[dim];

int fd; 
int pid, ppid;
int i=0;
msg buff;

void send(msg);
void receive(msg*);
void ricevuto(int);
void OK_to_send(int);
void fine(int);

int main(){
    int count=1;
    fd = creat("mailbox.tmp",0777); // 0777 == tutti possono fare tutto a sto file
    close(fd);
    fd=open("mailbox.tmp",O_RDWR);
    /*

        qualcuno si potrebbe chiedere perche abbiamo creato (e aperto) mailbox.tmp
        per poi richiuderlo, be quel qualcuno DOVREBBE SAPERE che la creat mi apre
        il file in sola scrittura, ignorante studia

    */
    
    signal(SIGUSR1,ricevuto);
    pid = fork();
    
    if (pid == 0){

        ppid = getppid();
        signal(SIGUSR1,OK_to_send);
        signal(SIGUSR2,fine);

        for(;;){
            sprintf(buff, "Ciao papa :( %2d\n",count++);
        }
    }else if (pid > 0){
        for (i=0; i<max;i++){
            receive(&buff);
        }
    }
}

void send(msg m){
    int n;
    lseek(fd,0,0); //mi resetto I/O pointer ==> file aperto dal padre ==> I/O pointer condiviso :(
    n = write(fd,m,dim);
    sleep(1); // !!!! necessario per assicurare sincronizazzione, non è il top
    kill(ppid,SIGUSR1);
    pause();
}
void receive(msg *m){
    int status;

    pause(); // aspetto che che figlio mi invii segnale (ovvero che ha scritto nel file)
    lseek(fd,0,0); // resetto I/O pointer (sto cazzo di I/O pointer condiviso)
    read(fd,m,dim); // leggo (non serviva manco commentare)
    write(1,m,15); // come ? ti stai chiedendo che significa ? dovresi sapere che 1 è l'fd di stdout
    if (i != max - 1 ){
        kill(pid, SIGUSR1); // se non sono ancora arrivato a max dico a mio figlio che puo andare ancora
    }else{
        close(fd);
        kill(pid,SIGUSR2); // altrimenti chiudo il file e gli dico basta
    }

}
void ricevuto(int signum) {
    char buff[25] = "ricevuto.. \n";
    write(1,buff,strlen(buff));
}

void OK_to_send(int signum){
    char buff[25] = "..fube receive\n";
    write(1,buff,strlen(buff));
}
void fine(int signum){
    char buff[25] = "FINE\n";
    close(fd);
    write(1,buff,strlen(buff));
    unlink("mailbox.tmp"); // cancello il file 
    exit(0); // sfanculo processo figlio
}
