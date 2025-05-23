#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>

#define MAX 100
#define NUMCOLOR 4
#define ASIZE 30
//P0 simula di essere la scatola ==> inizializza random il vettore di biglie
//1. stampo array random
//2. per ogni colore creo un processo che mi conta le biglie di quel colore

int trovaPos(int vet[], int dim, int key, int* pos)
{
	*pos = 0;
	int flag=0;
	for (int i = 0; i < dim && !flag; i++)
	{
		if (vet[i] == key)
		{
			*pos = i;
			flag = 1;
		}
	}
	return flag;
}

int findMax(int array[]){
    int max=0;
    for (int i=0;i<NUMCOLOR;i++){
        if (array[i]>max) max = array[i];
    }
    return max;
}
int getRandIndex(){

    return rand()%4;
}

int main(int argc, char *argv[]){
int num=0,status,numBiglie[NUMCOLOR],pid;
srand(time(NULL));
char colori_possibili[NUMCOLOR]={'r','g','b','y'};
char myArray[ASIZE];
for (int i=0; i<ASIZE; i++){
    myArray[i] = colori_possibili[getRandIndex()];
}

for (int i=0; i<ASIZE; i++){
    printf("%d: %c \n",i,myArray[i]);
}

/*
  come far ricordare al padre chi conta cosa?? posso usare un array in cui inserisco
  pid dei figli.. ?
*/
int pids[NUMCOLOR];
for (int i=0;i<NUMCOLOR; i++){
    num = 0; //resetto il mio counter ogni volta che entro qua dentro (dovrebbe essere ok? concorrenza molto carina su carta meno su vscode)
    pids[i] = fork();
    if (pids[i] == 0){
        printf("Sono il figlio con PID: %d, generato con i = %d\n",getpid(),i);
        /* 
           entro qua vuol dire che sono un figlio, la mia posizione nell'array pids mi
           influisce sul colore che devo contare, quindi se i==0 tecnicamente dovrei contare
           le rosse ? piu o meno (?), posso fare che se sono un figlio, controllo indice, in base
           a quell'indice scelgo che colore che contare e poi utilizzo magia nera per dire a p0
           le mie info, faccio una exit consi p1 non genera un figlio 
        */
       for (int j=0;j<ASIZE;j++){
        if (myArray[j] == colori_possibili[i]) num++; 
    
       }
        printf("Sono il figlio con PID: %d, ho contato %d bilgie di tipo %c\n",getpid(),num,colori_possibili[i]);
       exit(num);
    }
    if (pids[i] > 0){
        /*
            sono il padre, quindi uso wait per vedere che hanno fatto i miei figli
            uso indice per capire cosa ho contato
        */
       
       pid = wait(&status);
       if ((char)status == 0) { // lsb == 0 => term volontaria shifto per prendermi num
       printf("Sono il padre, PID: %d, ho appena ucciso mio figlio PID: %d\nLui ha contato %d biglie di tipo %c\n", getpid(),pid,status>>8,colori_possibili[i]);
       numBiglie[i] = status>>8;
       }else{
        perror("problemo no buono\n");
       }

    }
}
    int pos;
    trovaPos(numBiglie,NUMCOLOR,findMax(numBiglie),&pos); // il valore restituito non mi interessa perche so che l'elemento è presente (funzione riciclata da info 1)
    printf("Biglia piu presente: %c\n",colori_possibili[pos]);

    /*
        faccio una fork, poi una exec, altra magia -> exit e ho fatto circa
    */
   pid = fork();

   if (pid == 0){
    printf("%s\n",argv[1]);    
    char str[2] = {colori_possibili[pos],'\0'};
    printf("%s\n",str);
    execl("/bin/grep", "grep",str,(char *) 0);
    exit(0);
   }else if (pid > 0){
    pid = wait(&status);
    if (WIFEXITED(status)) 
        printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("Terminazione involontaria per segnale%d\n", WTERMSIG(status));
   }else if (pid<0)
    printf("Errore in fork! \n");
    

}