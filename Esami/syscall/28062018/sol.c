#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int pfd[2], pid[2], fdin, righe=0;

void p0_alrm_handler(int signo);
void processoP1(char* filename);
void p1_handler(int signo);
void processoP2(char* fileout);
void wait_child();

int main(int argc,char**argv){
	int Tout;
    //Controllo parametri
	if(argc!=4){
        fprintf(stderr,"Numero di argomenti errato.\nUsage:\n\t%s Fin N C\n", argv[0]);
        exit(EXIT_FAILURE);
	}	

    if( (Tout=atoi(argv[2]))<0  ){
        fprintf(stderr,"Intero N deve essere positivo\n");
        exit(EXIT_FAILURE);
    }

    //Esecuzione
    if(pipe(pfd)<0){
        fprintf(stderr,"Errore nella creazione della pipe tra P0 e P1\n");
        exit(EXIT_FAILURE); 
    }
        
    pid[0]=fork();
    if(pid[0]==0){//P1
        signal(SIGUSR1,p1_handler);
        printf("P1, pid = %d\n",getpid());
        close(pfd[0]);//chiudo lato di lettura
        processoP1(argv[1]);
        exit(EXIT_SUCCESS); 
    }
    else if(pid[0]>0){//P0
        pid[1]=fork();
        if(pid[1]==0){//P2
            printf("P2, pid = %d\n",getpid());
            close(pfd[1]);//chiudo lato di scrittura
            processoP2(argv[3]);
            exit(EXIT_SUCCESS); 
        }
        else if(pid[1]>0){//P0
            signal(SIGALRM,p0_alrm_handler);
        	alarm(Tout);
            close(pfd[0]);//chiudo lato di lettura
            close(pfd[1]);//chiudo lato di scrittura
            while(1){
				printf("P0: PID=%d\n",getpid());
				sleep(1);
			}
        }
        else{
            perror("P0: errore nella fork per P2\n");
            exit(EXIT_FAILURE); 
        }
    }
    else{
       perror("P0: errore nella fork per P1\n");
       exit(EXIT_FAILURE); 
    }
    
}


void p0_alrm_handler(int signo){
	int i;
	if (signo == SIGALRM){
		printf("P0: mando segnale a P1\n");
		kill(pid[0],SIGUSR1);
		// aspetto i figli e controllo il loro stato di terminazione in modo da 
    	//riscontrare eventuali loro malfunzionamenti
    	for(i=0;i<2;i++)
        	wait_child(); 
        printf("P0: termino.\n");
        exit(EXIT_SUCCESS);
	}else {
		perror("P0: segnale inatteso.\n");
		exit(EXIT_FAILURE); 
	}

}

void processoP1(char* filename){
	int seek_val,nread,written;
	char temp;

	fdin=open(filename,O_RDONLY);
    if(fdin<0){
        fprintf(stderr,"P1: Errore apertura file %s. Termino.\n",filename);
        exit(1); 
    }

    while(1){
	    /*Sposto avanti il cursore fino alla pipe fino al penutimo carattere del file */
	    seek_val = lseek(fdin, -1*sizeof(char), SEEK_END);
	    if ( seek_val < 0 ){
	        perror("P1: errore nella lettura del file di input\n");
	        close(fdin);
	        exit(EXIT_FAILURE);
	    }
		
	    while((nread=read(fdin,&temp,sizeof(char)))>0 && seek_val>=0){
	    	if (temp=='\n')
	    		righe++;
	        written=write(pfd[1],&temp,sizeof(char));
	        if(written<0){
	            perror("P1: Errore scrittura su pipe. Termino\n");
	            exit(EXIT_FAILURE); 
	        }
	        seek_val = lseek(fdin, -2*sizeof(char), SEEK_CUR);
	    }
    	sleep(1);
    }
}

void p1_handler(int signo){
	if (signo == SIGUSR1){
		printf("P1: ricevuto segnale SIGUSR1 da P0. In totale ho inviato %d righe. Ora termino.\n",righe);
		close(pfd[1]);
		close(fdin);
		exit(EXIT_SUCCESS);
	}else {
		perror("P1: segnale inatteso.\n");
		exit(EXIT_FAILURE);
	}

}

void processoP2(char *fileout){
	close(0);
    dup(pfd[0]);
    close(1);
    int fdout=open(fileout,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //int fdout=open(fileout,O_WRONLY);
    if(fdout<0){
        fprintf(stderr,"P2: Errore apertura file %s. Termino.\n",fileout);
        exit(1); 
    }
    execlp("cat", "cat", (char*)0);
    perror("P2: Errore nella exec. Termino.\n");
    exit(EXIT_FAILURE);
}

void wait_child() {
    int pid_terminated,status;
    pid_terminated=wait(&status);
    if(WIFEXITED(status))
        printf("P0: terminazione volontaria del figlio %d con stato %d\n",
                pid_terminated, WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("P0: terminazione involontaria del figlio %d a causa del segnale %d\n",
                pid_terminated,WTERMSIG(status));
}

