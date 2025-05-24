#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void waitMySonsDeath();
void checkInteger(int);
void termina(int);
int pid[2],fd[2];
int main(int argc, char *argv[]) {
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

    if (argc != 4){
        printf("Numero di parametri sbagliato..\n");
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '/'){
        printf("%s non è un path assoluto..\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    if ((open(argv[1],O_RDONLY))<0){
        printf("Ciao emh.. tu mi hai passato un file.. so che mi dovrei fidare..\nma ho comunque provato ad aprilo e non ci sono riuscito..\npuoi darmene uno che esiste?!\n");
        exit(EXIT_FAILURE);
    }
    checkInteger(N);
    checkInteger(M);

    if (pipe(fd)<0){
        printf("Creazione pipe fallita..esco\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGALRM,termina);
    
    
    pid[0] = fork();

    if (pid[0] == 0){
        
        int ffd;
        close(fd[0]);
        sleep(4);
        
        close(1);
        dup(fd[1]);
        close(fd[1]);
        

        /* ridirezione di output pipe su stdout */
        if ((ffd=open(argv[1],O_RDONLY))<0){
            printf("So che il file esiste ma non riesco ad aprirlo..esco\n");
            exit(EXIT_FAILURE);
        }
        printf("Figlio\n");
        char field[4];
        sprintf(field,"-f%s",argv[2]);
        printf("%s\n",field);
        printf("\' \'");

        execlp("cut", "cut", "-d"," ",field,argv[1],(char *)0);

        perror("Cut fallita: ");
        exit(EXIT_FAILURE);       
        
        /* P1 o fa exit a causa di exec o comunque fallisce */
    }

    pid[1] = fork();

    if (pid[1] == 0){
        close(fd[1]);

        char c,word[20];
        int wordCounter=0,i=0;
        while (read(fd[0],&c,sizeof(char))>0){
            word[i++]=c;

            if (c == '\n'){
                word[i-1] = '\0';
                /* sostiusco \n non terminatore di stringa, altrimetni ho problemi nella stampara
                n.b. sempre meglio metterlo sto cazzo di /0 !!
                */
                printf("parola %i : %s\n",wordCounter,word);
                wordCounter++;
                for (int j=0;i<strlen(word);j++) word[j] = '\0';
                /* resetto a mano stringa, potevo usare memset ma chill */
                i=0;
            }
            
        }

        exit(EXIT_SUCCESS);
    }



    if (pid[0]>0 && pid[1]>0){
        close(fd[0]);
        close(fd[1]);
        alarm(M);

        for (int i = 0; i < 2; i++) waitMySonsDeath();


        exit(EXIT_SUCCESS);
    }



    


}
void termina (int signum){
    printf("Timeout scaduto..\n");
    fflush(stdout);
    kill(pid[0],SIGKILL);
}
void checkInteger(int integer){
    if (integer <= 0){
        printf("%i non è un intero positivo..\n",integer);
        exit(EXIT_FAILURE);

    }
}

void waitMySonsDeath()
{
    int pid, status;
    pid = wait(&status);
    if (WIFEXITED(status))
        printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Terminazione involontaria per segnale %d\n", WTERMSIG(status));
}

