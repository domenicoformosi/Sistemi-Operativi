#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int pfd[2], pid[2];
char C[2];
char ftemp[15];

void processoP0(int pfd_in);
void processoP2(char* fin, int N);
void p1_handler(int signo);
void wait_child();

int main(int argc,char**argv){

    int N;
	//Controllo parametri
	
	if(argc!=4){
		fprintf(stderr,"Numero di argomenti errato\n");
        fprintf(stderr,"Usage:\n\t%s Fin N C\n", argv[0]);
        exit(1);
	}	
    if(argv[1][0] != '/') {
        printf("%s non e' un percorso assoluto!\n", argv[1]);
        exit(-2);
    }


    if( (N=atoi(argv[2]))<0  ){
        fprintf(stderr,"Intero N deve essere positivo\n");
        exit(1);
    }
    if(strlen(argv[3])>1){
        fprintf(stderr,"C deve essere un carattere\n");
        exit(1);
    }
    C[0]=argv[3][0];
    C[1]='\0';

    sprintf(ftemp,"%s","/tmp/temp");
    

    if(pipe(pfd)<0){
        fprintf(stderr,"Errore nella creazione della pipe tra P0 e P1\n");
        exit(1);
    }
    
        
    pid[0]=fork();
    if(pid[0]==0){//P1
        printf("P1, pid = %d\n",getpid());
        close(pfd[0]);//chiudo lato di lettura
        signal(SIGUSR1,p1_handler);
        pause();
        exit(0);
    }
    else if(pid[0]>0){//P0
        pid[1]=fork();
        if(pid[1]==0){//P2
            printf("P2, pid = %d\n",getpid());
            close(pfd[0]);//chiudo lato di lettura
            close(pfd[1]);//chiudo lato di scrittura

            processoP2(argv[1],N);
            exit(0);
        }
        else if(pid[1]>0){//P0
            close(pfd[1]);//chiudo lato di scrittura
            processoP0(pfd[0]);
        }
        else{
            fprintf(stderr,"P0: errore nella fork per P2\n");
            exit(1); 
        }
    }
    else{
       perror("P0: errore nella fork per P1\n");
        exit(1); 
    }
    
}


void processoP0(int pfd_in){
    int i,nread,fdtemp,fdin,written,crighe = 0;
    char temp;

    while((nread=read(pfd_in,&temp,sizeof(char)))>0){
        printf("%c",temp);
    }
    if(nread<0){
        perror("P0: Errore nella lettura da pipe");
        exit(1); 
    }
    // aspetto i figli e controllo il loro stato di terminazione in modo da 
    //riscontrare eventuali loro malfunzionamenti
    for(i=0;i<2;i++)
                wait_child(); 
}
/* codice alternativo per P0
void processoP0(int pfd_in){
    close(0);
    dup(pfd_in);
    execlp("cat", "cat", (char*)0);
    perror("P0: errore nella exec\n");
    exit(3);
    //NB: LA EXEC NON HA RITORNO SE NON IN CASO DI ERRORE. Dopo la exec c'è sempre una perror e una exit!
}*/

void processoP2(char* fin, int N){
    int nread,fdtemp,fdin,written,crighe = 0;
    char temp;

    fdin=open(fin,O_RDONLY);
    if(fdin<0){
        fprintf(stderr,"P2: Errore apertura file %s\n",fin);
        exit(1); 
    }
    fdtemp=open(ftemp,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fdtemp<0){
        fprintf(stderr,"P2: Errore apertura file %s\n",ftemp);
        exit(1); 
    }
    while((nread=read(fdin,&temp,sizeof(char)))>0){
        if(crighe%N==0){
            written=write(fdtemp,&temp,sizeof(char));
            if(written<0){
                fprintf(stderr,"P2: Errore scrittura su file %s\n",ftemp);
                exit(1); 
            }
        }
        if(temp=='\n'){
            crighe++;   
        }
    }
    if(nread<0){
        fprintf(stderr,"P2: Errore nella lettura da file %s\n",fin);
        close(fdin);
        exit(1); 
    }
    kill(pid[0],SIGUSR1);
}

void p1_handler(int signo){
    int fdtemp;
    if(signo==SIGUSR1){
        printf("P1: Ho ricevuto SIGUSR1...");
        close(1);
        dup(pfd[1]);
        execlp("grep", "grep", C , ftemp, (char *)0);
        perror("P1: errore nella exec\n");
        exit(3);
    }else{
        perror("P1: Segnale inaspettato\n");
        exit(-2);
    }
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

