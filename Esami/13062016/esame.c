#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int isEven();
void printEven(int);
void printNotEven(int);
void killSon(int);
void waitMySonsDeath();

int fd[2],pid[2],filed;
char fin[100],fout[100];
int main (int argc, char * argv[]){

    if (argc != 5){
        printf("Numero di parametri in input illegale! %i\n",argc);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '/' || argv[2][0] != '/'){
        printf("Uno dei due path non è assoluto (magari entrambi)\n");
        exit(EXIT_FAILURE);
    }
    if (open(argv[1],O_RDONLY) <  0){
        printf("Broo %s non esiste!!!\n",argv[1]);
        exit(EXIT_FAILURE);
    }
     if (open(argv[2],O_RDONLY) >= 0){
        printf("Broo %s esiste (non dovrebbe) !\n",argv[2]);
        exit(EXIT_FAILURE);
    }  
    if (strlen(argv[3])>1)  {
        printf("Non mi hai passato il carattere!\n");
        exit(EXIT_FAILURE);
    }
    if (argv[3][0] != 'P' && argv[3][0] != 'D' ) {
        printf("C è diverso da P o D!\n");
        exit(EXIT_FAILURE);
    }
    if (atoi(argv[4])<=0){
        printf("T <= 0 !\n");
        exit(EXIT_FAILURE);
    }

    strcpy(fin,argv[1]);
    strcpy(fout,argv[2]);

    signal (SIGUSR1,printEven);
    signal (SIGUSR2,printNotEven);
    signal(SIGALRM,killSon);
    alarm(atoi(argv[4]));
    pipe(fd);
    printf("sono gay\n");
    pid[0] = fork();

    if (pid[0] == 0){
        close(fd[1]);
   
        char num[10];
        int i=0,value;
        while (read(fd[0],&num[i],sizeof(char)) > 0 &&  num[i]!= ' '){
            i++;
        }
        num[i] = '\0';
        value = atoi(num);
        printf("%i\n",value);
        printf("%c\n",argv[3][0]);
        if (isEven(value) && argv[3][0]== 'P'){
            printf("tutto pari\n");
            kill(getppid(),SIGUSR1);
        }else if (!isEven(value) && argv[3][0]== 'D'){
            printf("tutto dispari\n");
            kill(getppid(),SIGUSR2);   
        }
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    pid[1] = fork();

    if (pid[1] == 0){
        close(fd[0]);

        close(1);
        dup(fd[1]);
        close(fd[1]);

        execlp("wc","wc", "-l",argv[1],(char *)0);
        perror("Exec fallita: ");
        exit(EXIT_FAILURE);
    }

    if (pid[0] > 0 || pid[1] >0){

        filed = creat(argv[2],0666);
        if (filed<0){
            printf("apertura fallita!\n");
        }
    


        
        for (int i=0; i<2;i++){
            waitMySonsDeath();
        }
        exit(0);
    }





}

void killSon(int signum){
    kill(pid[0],SIGKILL);
}
void printEven(int signum){
    printf("%c contiene un numero pari di righe",fin);
    
    char str[100];
    strcpy(str,fin);
    strcat(str," contiene un numero pari di righe");
    write(filed,&str,sizeof(str));
  
}
void printNotEven(int){
    printf("%c contiene un numero pari di dispari",fin);
}


int isEven(int value){
    return (value % 2) == 0 ? 1 : 0;
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