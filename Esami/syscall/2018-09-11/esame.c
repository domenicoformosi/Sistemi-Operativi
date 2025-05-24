#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void stampa(int);
void waitMySonsDeath();
int fd, pipefd[2], pid[2],fout;
char c;
int main(int argc, char *argv[])
{

    int N = atoi(argv[2]);
    if (argc != 4)
    {
        printf("Numero di parametri in input errato\n");
        exit(EXIT_FAILURE);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        printf("Il file non esiste!\n");
        exit(EXIT_FAILURE);
    }
    close(fd);

    if ((fd = open(argv[3], O_RDONLY))>0)
    {
        printf("Il file fout esiste! (non dovrebbe)\n");
        exit(EXIT_FAILURE);
    }

    if (N <= 0)
    {
        printf("numero non valido!\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) < 0)
    {
        printf("Pipe non riuscita!\n");
        exit(EXIT_FAILURE);
    }

    if ((fout = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC))<0)
    {
        printf("Non sono riuscito a creare fout\n");
        exit(EXIT_FAILURE);
    }
    pid[0] = fork();

    if (pid[0] == 0)
    {
        close(pipefd[0]);

   
        if ((fd = open(argv[1], O_RDONLY)) < 0)
        {
            printf("Problemi apertura del file!!\n");
            exit(EXIT_FAILURE);
        }
        int i;
        while ((i=read(fd,&c,sizeof(char)))>0)
        {
            write(pipefd[1],&c,sizeof(char));   
            lseek(fd,N-1,SEEK_CUR);
            /* mi sposto avandi direttamente di N invece di leggere tutto e controllare se sia multiplo*/
        }

        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    }

    pid[1] = fork();

    if (pid[1] == 0)
    {
        signal(SIGUSR1,stampa);
        close(pipefd[0]);
        close(pipefd[1]);

        pause();
        /* stampa a video contenuto di fout usando cut (?)*/

    }

    if (pid[0] > 0 && pid[1] > 0)
    {
        close(pipefd[1]);
        int stdout_fd = dup(1);

        close(1);
        dup(fout);
        close(fout);

        /* se chiudo stdout e subito dopo apro file il suo fd dovrebbe essere 1*/


        while (read(pipefd[0],&c,sizeof(char)))
        {
            printf("A");
            printf("%c",c);
        }
        
        
        kill(pid[1],SIGUSR1);


        close (1);
        dup(stdout_fd);

        sleep(1);
        printf("\n");
        for (int i=0;i<2;i++) waitMySonsDeath();

        exit(EXIT_SUCCESS);


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


void stampa (int signum)
{
    /* stampo con cut */


    execlp("cat","cat",fout,(char *)0);



    exit(EXIT_SUCCESS);
}
