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
void termina(int);
void waitTillTheyDie();

int fd,pid[2],pipefd[2],t;
char *fin;
char c;
int main(int argc, char* argv[])
{
	c = argv[2][0];
	printf("Carattere da cercare %c\n",c);
	t = atoi(argv[3]);
	fin = argv[1];
	if (argc != 4)
	{
		printf("Numero di argomenti sbagliato!\n");
		exit(EXIT_SUCCESS);
	}

	if ((fd=open(argv[1],O_RDONLY))<0)
	{
		printf("Non riesco ad aprire il file!!!\n");
        exit(EXIT_SUCCESS);
	}
	close(fd);
	if (t <= 0)
	{
		printf("Parametro non positivo!!!\n");
        exit(EXIT_SUCCESS);
	}
	

	if (pipe(pipefd)<0)
	{
		perror("Errore pipe: ");
		exit(EXIT_FAILURE);
	}
	signal(SIGUSR1,stampa);
	signal(SIGALRM,termina);

	pid[0] = fork();

	if (pid[0] == 0)
	{
		int numOcc = 0;
		char buff;
		close(pipefd[0]);
		
		if ((fd = open(argv[1],O_RDONLY))<0)
		{	
			printf("P1: Problema nel (ri)aprire il file %s\n",argv[1]);
			exit(EXIT_FAILURE);
		}
		while (read(fd,&buff,sizeof(char))>0)
		{

			if (buff == c)
				numOcc++;
		}

		if (write(pipefd[1],&numOcc,sizeof(numOcc))< 0)
		{
			printf("P1: Problema nell'inviare numero di occorrenze a P2!\n");
			exit(EXIT_FAILURE);
		}	
		printf("P1:Inviato %i a P2\n",numOcc);

		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}

	pid[1] = fork();
	if (pid[1] == 0)
	{
		int numChar=0, numOcc,i=0,rd;
		char buff,buffer[5];
		close(pipefd[1]);

		if ((fd = open(argv[1],O_RDONLY))<0)
		{	
			printf("P2: Problema nel (ri)aprire il file %s\n",argv[1]);
			exit(EXIT_FAILURE);
		}

		while ((rd=read(fd,&buff,sizeof(char)))>0) //leggo un carattere la volta uso un solo char
		{
			/* assumendo che lo spazio e \n siano un carattere, in caso basta aggiungere condizione*/
			numChar++;
			/*
			numChar = buff == ' ' ? numChar : (buff == '\n' ? numChar : ++numChar)			
			*/
		}
		//sleep(2);
		if (rd<0)
		{
			printf("P2: problema nella lettura del file !\n");
			exit(EXIT_FAILURE);
		}

		rd=read(pipefd[0],&numOcc,sizeof(int));
		
		if (rd<0)
		{
			printf("P2: problema nella lettura della pipe !\n");
			exit(EXIT_FAILURE);
		}

		printf("P2: ho letto %i caratteri e P1: %i\n",numChar,numOcc);
		if (numOcc > numChar/2)
		{
			kill(getppid(),SIGUSR1);
		}

		exit(EXIT_SUCCESS);
	}

	if (pid[0]>0 && pid[1]>0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		alarm(t);
		for (int i=0;i<2;i++) waitTillTheyDie();


		exit(EXIT_SUCCESS);
		
	}



}
void stampa(int)
{
	printf("%s contiene piu del 50 percento \A di caratteri di %c\n",fin,c);
}
void termina(int)
{
	printf("Allarme scaduto, termino l'esecuzione di tutti..\n");
	fflush(stdout);
	kill(0,SIGKILL);
	exit(EXIT_SUCCESS);
}
void waitTillTheyDie()
{
    int pid, status;
    pid = wait(&status);
    if (WIFEXITED(status))
        printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Terminazione involontaria per segnale %d\n", WTERMSIG(status));
}
