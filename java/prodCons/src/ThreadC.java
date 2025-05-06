public class threadP extends Thread{ //produttori
int i=0;
risorsa r; //oggetto che rappresenta il buffer condiviso
public threadP(risorsa R)
{ this.r=R;
}
public void run()
{ try{ System.out.print("\nThread PRODUTTORE: il mio
ID è: "+getName()+"..\n");
while (i<100)
{ sleep(100);
r.inserimento(i);
i++;
System.out.print("\n"+ getName() +":
inserito messaggio " +i+ "\n");
}
}catch(InterruptedException e){}
}
}