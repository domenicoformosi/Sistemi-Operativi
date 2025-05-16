#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define INPUTERROR -1
int fd[2], M;

void mkcommand(char *[], char[], char[]);
void divisibile(int);
void nonDivisibile(int);
void waitMySonsDeath();
int main(int argc, char *argv[])
{

    if (argc < 4)
    {
        printf("Numero di parametri in input illegale! \n");
        exit(INPUTERROR);
    }

    M = atoi(argv[3]);
    int pid[2], nlines, i = 0;
    char num[4];

    if (access(argv[1], F_OK) < 0)
    {
        printf("Impossibile aprire il file!\n");
        exit(INPUTERROR);
    }

    if (M <= 0)
    {
        printf("Numero illagel!\n");
        exit(INPUTERROR);
    }
    if (argv[1][0] != '/')
    {
        printf("Path non assoluto!\n");
        exit(INPUTERROR);
    }

    pipe(fd);
    pid[0] = fork();

    if (pid[0] == 0)
    {

        
        close(fd[0]);

        /*
            chiudo lato di lettura della pipe --> non mi serve
        */

        close(1);
        dup(fd[1]);
        close(fd[1]);

        /*
            ridireziono fd[1] su stdout
        */

        execl("/bin/grep", "grep", "-c", "-w", argv[2], argv[1], (char *)0);
        perror("Exec fallita: ");
        exit(EXIT_FAILURE); // se sto eseguendo questo codice significa execl fallita
    }

    pid[1] = fork();

    if (pid[1] == 0)
    {
        signal(SIGUSR1, divisibile);
        signal(SIGUSR2, nonDivisibile);
        /*
            ridefinisco comporamento dei due segnali
        */
        close(fd[0]);
        close(fd[1]);
        /*
            questa parte anche se non sembra è fondamentale, P2 non deve chiudere i lati di scrittura solamente perche
            non li usa e quindi è giusto chiuderli, ma perche il padre fa una read sulla pipe, una volta che legge la
            roba che gli serve la pipe è vuota, cio implica che se tutti i processi l'hanno chiusa ritornera EOF, 
            se non chiudi scrittura sulla pipe esecuzione non termina!!!
        */
        pause();
        /*
            mi metto in attesa dei segnali di P0
        */
        exit(0);
    }

    if (pid[1] > 0)
    {
        close(fd[1]);
        int nread;
        printf("sono padre \n");
        while ((nread = read(fd[0], &num[i], sizeof(char))) > 0)
        {
            i++;
        }
        num[i] = '\0';
        /*
            Questo tipo di lettura è necessaria perche la grep mi butta nel buffer una stringa, non posso
            quindi leggere un intero!!!
        */
        close(fd[0]);
        nlines = atoi(num);
        printf("%s\n", num);
        if ((nlines % M) == 0)

            kill(pid[1], SIGUSR1);

        else
            kill(pid[1], SIGUSR2);

        for (int i=0;i<2;i++){
            waitMySonsDeath;
        }
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
void divisibile(int signum)
{

    printf("Sono P2, il mio pid è: %i. Il numero di occorrenze è divisibile per %i\n", getpid(), M);
    exit(0);
}
void nonDivisibile(int signum)
{
    printf("Sono P2, il mio pid è: %i. Il numero di occorrenze non è divisibile per %i\n", getpid(), M);
    exit(0);
}
