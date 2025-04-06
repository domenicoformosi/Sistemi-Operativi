
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define Nmax 50
#define Mdim 8
int fd, pipefd[2], L;
char c, msg[Mdim];
void figlio(int ind, char c, int fra, char filein[]);
int main(int argc, char **argv)
{
    int pid[Nmax], N, stato, i, tot, K, p, fraz;
    if (argc != 4)
    {
        printf("sintassi!\n");
        exit(-1);
    }

    N = atoi(argv[2]);
    printf("Creerò %d figli!\n", N);
    c = argv[3][0];
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open padre");
        exit(-2);
    }
    L = lseek(fd, 0, SEEK_END);
    /*
        che sta succedendo qua ? in pratica lseek mi restituisce pos I/O pointer
        se la metto alla fine con offset 0 ottengo dim.
    */
    fraz = L / N;
    close(fd);
    /*
    chiudo il file perchè non voglio condividire I/O pointer, ognuno il suo altrimenti
    devo impazzire con lseek
    */

    if (pipe(pipefd) < 0)
        exit(-3); /* apertura pipe */

    /* creazione figli: */
    for (i = 0; i < N; i++)
        pid[i] = fork();
    if ((pid[i] = fork()) < 0)
    {
        perror("fork");
        exit(-3);
    }
    else if (pid[i] == 0)
    {
        printf("Sono figlio %d di indice %d \n", getpid(), i);
        figlio(i, c, fraz, argv[1]);
    }
    else
    {
        /* padre */
        close(pipefd[1]); // chiudo in scrittura
        for (tot = 0, i = 0; i < N; i++)
        {
            read(pipefd[0], &K, sizeof(int)); // lettura binaria
            tot += K;
        }
        close(pipefd[0]);
        printf("\n valore ottenuto: %d\n", tot);
        for (i = 0; i < N; i++)
        {
            p = wait(&stato);
            if (WIFEXITED(stato))
                printf("Term. volont. di %d con stato %d\n", p, WEXITSTATUS(stato));
            else if (WIFSIGNALED(stato))
                printf("term. Involont. di %d per segnale %d\n", p, WTERMSIG(stato));
        }
        exit(0); /* fine padre */
    }
} /* fine main */

void figlio(int ind, char c, int fra, char filein[])
{

    int i, j, cont = 0;
    char car;
    close(pipefd[0]);
    fd = open(filein, O_RDONLY); /*I/O pointer privato */
    lseek(fd, ind * fra, 0);     /*posiz. inizio frazione */
    for (j = 0; j < fra; j++)
    {
        i = read(fd, &car, 1);
        if (i == 0)
            break;
        if (car == c)
            cont++;
    }
    write(pipefd[1], &cont, sizeof(int));
    close(pipefd[1]);
    close(fd);
    exit(0);
}