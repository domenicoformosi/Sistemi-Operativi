public class threadC extends Thread{ //consumatori
int msg;
risorsa r;
public threadC(risorsa R)
{ this.r=R;
}
public void run()
{ try{ System.out.print("\nThread CONSUMATORE: il mio
ID è: "+getName()+"..\n");
while (true)
{ msg=r.prelievo();
System.out.print("\n"+getName()+"
consumatore ha letto il messaggio "+msg
+ "...\n");
}
}catch(InterruptedException e){}
}
}