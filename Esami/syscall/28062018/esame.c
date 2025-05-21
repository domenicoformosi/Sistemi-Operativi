#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
void checkInteger(int);
void avvisa(int);
void muori(int);
int pfd[2],pid[2],n=0;
int main(int argc, char *argv[]){
    int Tout = atoi(argv[2]),fd;

    if (argc != 4){
        printf("Numero di parametri sbagliato..\n");
        exit(EXIT_FAILURE);
    }

    if ((fd=open(argv[1],O_RDONLY))<0){
        printf("Ciao emh.. tu mi hai passato un file.. so che mi dovrei fidare..\nma ho comunque provato ad aprilo e non ci sono riuscito..\npuoi darmene uno che esiste?!\n");
        exit(EXIT_FAILURE);
    }
    close(fd);
    checkInteger(Tout);

    if ((fd=open(argv[3],O_RDONLY))>0){
        printf("Il file esiste :( \n");
        exit(EXIT_FAILURE);

        // capire perche non va
    }
    close(fd);
    signal(SIGALRM,avvisa);
    pid[0] = fork();

    if (pid[0] == 0){
        signal(SIGUSR1,muori);
        char c;
        int lseekReturn=1;
        close(pfd[0]);
        if ((fd=open(argv[1],O_RDONLY))<0){
            exit(EXIT_FAILURE);
        }

        while (1){
            lseek(fd,-1,SEEK_END);

            while (read(fd,&c,sizeof(char))>0 && lseekReturn > 0){

                write(pfd[1],&c,sizeof(char));
                n = c == '\n' ? n++ : n;

                lseekReturn=lseek(fd,-2 * (sizeof(char)),SEEK_CUR);
                /* se la read mi va avanti con io pointer e la lseek indietro di uno forse con lseek devo andare indietro di "?"*/
            }
        }

    }

    pid[1] == fork();

    if (pid[1] == 0){
        close(pfd[1]);
        char c;
        
        if ((fd=open(argv[3],O_WRONLY | O_CREAT))<0){
            exit(EXIT_FAILURE);
        }
        close(1);
        dup(fd);
        close(fd);


        while (read(pfd[0],&c,sizeof(char))>0){
            execlp("cat","cat",c);
            /* bho nel senso la exec non dovrbbe terminare??*/
        }
    }

    if (pid[1] > 0 && pid[0]> 0){
        alarm(Tout);
        


    }


}
void avvisa(int signum){
    kill(pid[0],SIGUSR1);
    exit(EXIT_SUCCESS);
}

void muori(int signum){
    printf("P1: in totale ho inviato %m volte il carattere newline a P2",n);
    fflush(stdout);
    exit(EXIT_SUCCESS);

}
void checkInteger(int integer){
    if (integer <= 0){
        printf("%i non è un intero positivo..\n",integer);
        exit(EXIT_FAILURE);

    }
}