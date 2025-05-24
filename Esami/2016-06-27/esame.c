#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int fd, pipefd[2], pid[2];
char C[2],temp[10];
void p1 (int);

int main(int argc, char *argv[])
{

    /*
        1 parametro: nome assoluto di un file di testo esistente nel file system
        2 parametro: numero intero positivo
        3 parametro: un carattere
    */
    int n = atoi(argv[2]),i = 0, numRiga = 1, tempFd;;
    char buffer[1000], buff;
    strcpy(temp, "/tmp/temp");
    if (argv[1][0] != '/')
    {
        printf("No! %s non è un path assoluto!!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    close(fd);

    if (n <= 0)
    {
        printf("No! %s non è un intero positivo!!\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[3]) > 1)
    {
        printf("No! %s non è un singolo carattere!!\n", argv[3]);
        exit(EXIT_FAILURE);
    }
    C[0] = argv[3][0];
    C[1] = '\0';
    pipe(pipefd);

    pid[0] = fork();

    if (pid[0] == 0)
    {
        signal(SIGUSR1,p1);
        pause();
        
    }

    pid[1] = fork();

    if (pid[1] == 0)
    {
        /*
            P2: leggo fin a righe multiple di n e scrivo contenuto in /tmp/temp, quando finisco avviso P1
        */

        close(pipefd[0]);
        close(pipefd[1]);
        
        
        if ((tempFd = open(temp, O_WRONLY)) > 0)
        {
            unlink(temp);
            printf("file esisteva, l'ho cancellato..\n");
        }

        tempFd = creat(temp, 0777);
        printf("%i\n",tempFd);
        close(tempFd);

        if ((tempFd = open(temp, O_WRONLY)) > 0){
            printf("creato file tmp\n");
        }else { printf("Errore creazione tmp");}

        if ((fd = open(argv[0], O_RDONLY)) > 0)
        {
            printf("aperto fin\n");
            while ( read( fd, &buff, sizeof(char) ) > 0)
            {
               if(numRiga%n == 0){
                write(tempFd,&buff,sizeof(char));
               }
               numRiga = buff == '\n' ? numRiga++ : numRiga;
            } 
            
        }
        printf("finita analisi fin\n");
        kill(pid[0],SIGUSR1);
        exit(EXIT_SUCCESS);
    }

    if (pid[0] > 0 || pid[1] > 0){

        close(pipefd[1]);
        while (read(pipefd[0],&buffer[i],sizeof(char)) > 0){
            printf("%c",buffer[i]);
        }

        printf("%s",buffer);

        exit(0);
    }
}
 void p1(int signum){
        printf("handler p1\n");
        close(pipefd[0]);

        close(1);
        dup(pipefd[1]);
        close(pipefd[1]);

        execlp("grep", "grep",C,temp, (char *)0);
        perror("Grep fallita: ");
        exit(EXIT_FAILURE);

 }