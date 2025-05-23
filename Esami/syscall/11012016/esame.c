#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Numero di argomenti sbagliato!\n");
		exit(EXIT_SUCCESS);
	}

	int fd;

	if ((fd=open(argv[1],O_RDONLY))<0)
	{
		printf("Non riesco ad aprire il file!!!\n");
                exit(EXIT_SUCCESS);
	}
	
	close(fd);





}
