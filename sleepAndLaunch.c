#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


void checkInput(int,int, int); // controlla validità parametri in input 
void alrmHandler(int);
void sr1Handler(int);
void debugHandler(int);

int main(int argc, char * argv[]){
int pid,status,k,S = atoi(argv[1]),R = atoi(argv[2]);
checkInput(argc,S,R);

/*
    P0 deve inoltre controllare che l’esecuzione non superi mai i 3
    secondi. Pertanto, trascorso tale tempo dall’inizio della sua
    esecuzione, P0 deve stampare la stringa <<Timeout scaduto!>>,
    terminare P1 e terminare a sua volta.
    uso alarm(?) -> devo ridefinire handler per P0
*/
alarm(3); //avvio timer
signal(SIGALRM,alrmHandler); //ridefinisco comportamento come da specifica
signal(SIGUSR1,sr1Handler); //ridefinisco comportamento come da specifica
pid = fork();

if (pid == 0){
    signal(SIGUSR2,debugHandler);
    /*
        per prima cosa dormo S secondi --> uso una sleep
    */
   k = sleep(S);
   if (R == 0){ 
    kill(getppid(),SIGUSR1);
    exit(EXIT_SUCCESS);
    }
    else {
        /* faccio creare a P1 un figlio che mi gestisce la exec x whoami*/
        int pid1 = fork();
        if (pid1 == 0){
            execl("/bin/whoami","whoami",(char*)NULL);
            perror("Exec fallita: ");
            exit(EXIT_FAILURE); // se sto eseguendo questo codice significa execl fallita
        }else if (pid1> 0){
            pid1 = wait(&status);
            if ( WIFEXITED(status)) printf("Comando whoami eseguito con successo!\nTerminazione volontaria di %d con stato %d\n", pid1, WEXITSTATUS(status));
            else if (WIFSIGNALED(status)) printf("Terminazione involontaria per segnale %d\n",WTERMSIG(status));
        }else{
            /* fork fallita dovrei farci qualcosa*/
        }

    }
}else if (pid>0){
    pid = wait(&status);
    if ( WIFEXITED(status)) printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) printf("Terminazione involontaria per segnale %d\n",WTERMSIG(status));

}



}

void checkInput(int nArg,int S, int R){
    if (nArg != 3 || S < 0 || (R<0 || R>1)) exit(EXIT_FAILURE);
}
void alrmHandler(int signum){
    printf("Timetout scaduto!!!\n");
    /* devo capire come terminare P0 e P1, come trovo pid P1 nell'handler? p.s. uso var. globali*/
    kill(0, SIGKILL);
    

}
void sr1Handler(int signum){
    printf("Ricevuto il segnale da P1!!!\n");
}

void debugHandler(int signum){
    printf("Sono: %d ho ricevuto un segnale (doveva essere una kill)\n",getpid()); //funz. di debug
}
