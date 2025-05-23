#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv []) {
printf("O\n");
int fd = open("test",O_RDONLY | O_CREAT );
if (fd<0) printf("Problemo!\n");

char c;
int eof=lseek(fd,-1,SEEK_END);
printf("%i\n",eof);
while (read(fd,&c,sizeof(char))>0 && eof >= 0){
    printf("Leggo..\n");
    printf("..carattere letto: %c\n",c);




    //eof = (eof == 1) ? lseek(fd,-1,SEEK_CUR) : lseek(fd,-2,SEEK_CUR);
    
    /*
        quello che mi permette di uscire dalla lettura in questo caso non è eof, o meglio, lo è, ma la variabile
        non la eof restituita dalla read, devo essenzialemnte giocare con il pointer, parto a leggere dall'ultimo 
        carattere e poi torno indietro di 2 (altrimenti rileggo stesso carattere), importante eof>= 0 altrimenti non
        leggere primo carattere
    */
    
    eof = lseek(fd,-2,SEEK_CUR);
    printf("%i\n",eof);
    
    
}






}