#include <stdio.h>
#include <signal.h>
int cont=0;
void handler(int signo)
{ printf ("Proc. %d: ricevuti n. %d segnali %d\n", getpid(),cont++, signo);
}
main ()
{int pid;
signal(SIGUSR1, handler);
pid = fork();
if (pid == 0) /* figlio */
for (;;);
else /* padre */
for(;;) kill(pid, SIGUSR1);
}