#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/*
    prende come paramentro argc, so che il numero di file è argc - 3 e poi me lo ritorno
*/
int argumentsCheck(int);

/*
    la devo spiegare veramente?
*/
int isEven(int);

int main(int argc, char *argv[]){
int nFile = argumentsCheck(argc),pid,ppid,status,counter=0; 
char* dir_1 = argv[1];
char* dir_2 = argv[2];
ppid = getpid();
for (int i=0;i<nFile;i++){
    pid = fork();
    if (pid == 0){
        // generato n-esimo figlio: comportamento da differenziare in base al valore del pid del padre
        

        char src[strlen(argv[i+3])+6];// ma sta roba è legale ? non è che mi serve una malloc?
        strcpy(src,"dir1/");
        printf("%s \n", src);
        strcat(src,argv[i+3]);
        printf("%s \n", src);
        strcat(src,"\0");
        printf("%s \n", src);
        if (isEven(ppid)){
            /* 
                produco copia dell'i-esimo file nella seconda directory
                devo creare la stringa di destinazione dir1/nomefile 
                per dimensionare l'array sono 5 char + strlen(nomefile) + terminatore
                l'i-esimo file lo trovo facendo i+3 (se i=0 il primo file lo trvo a argv[3]) 
                p.s. spostato di sopra
            */
            execl("bin/ls", "ls", "",(char*)0);
            execl("/bin/cp","cp",src,"dir2",(char*)0);

            exit(0);
        }else{
            /* 
                tolgo l'i-esimo file dalla prima directory
            */

            execl("/bin/rm","rm",src,(char*)0);

           exit(0);
        }

    }else if (pid > 0){
        /*
            sono padre, se tutti i miei figli terminano volontariamente ( come controllo sta roba? 
            p.s. uso libreria wait.h) sparo una ls nella seconda directory, altrimenti error msg con ppid
        */
       pid = wait(&status);
       if (WIFEXITED(status)){

        printf("diocane \n");
        counter ++; //un pò naive (conto quanti figli terminano volontariamente) ? puo essere.
       }
    }else printf("fork fallita per qualche motivooo \n");

    printf("nfile: %d counter: %d", nFile,counter);
    if (counter == nFile) execl("/bin/ls","ls","",(char *)0);
    else printf("Uno dei figli ha terminato la sua esecuzione involontariamente, pid padre: %d \n",getpid());
}
/*
    devo finire dopo, cose da ricordarmi:
    1- c'è qualche problema con il controllo della terminazione volontaria, possibile sol spostarla nel ramo del padre
    cambiando un po logica di controllo
*/



}

int argumentsCheck (int argc){
    if ((argc-3) == 0) exit(0);
    return argc-3;
}

int isEven(int num){
    return (num%2)==0 ? 1 : 0;
}