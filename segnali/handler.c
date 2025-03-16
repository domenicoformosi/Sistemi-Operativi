#include <signal.h>
#include <stdio.h>
void handler(int);


/*
    per provare questo codice runna l'eseguibile in background (dai bro lo sai fare),
    poi invia uno dei due segnali da terminale (rispettuivamente 10 e 12) e niente
    questo è, che cosa ti aspetti di piu

*/
int main(){
    if (signal(SIGUSR1,handler) == SIG_ERR) perror("prima signal non riuscita! \n");
    if (signal(SIGUSR2,handler)==SIG_ERR) perror("prima signal non riuscita! \n");

    for (;;); //aspetto segnali da terminale

}


void handler (int signum){
if (signum == SIGUSR1) printf("ricevuto 1 !!! \n");
else if (signum == SIGUSR2) printf("ricevuto 2 !!! \n");


}