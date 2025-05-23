package es10;
import java.util.concurrent.locks.*;

public class Fattoria {
	
	@Override
	public String toString() {
		return "Visitatori totali: "+totVisite+", incassi totali: "+totIncasso+"€\n";
	}

	private int capMax,capNowBot,capNowZoo,nCompl,pBot,pZoo,sospBot,sospZoo,totVisite,totIncasso;
	private Lock lock = new ReentrantLock();
	Condition ZooCond;
	Condition BotCond;
	
	
	public Fattoria(int capMax, int nCompl, int pBot, int pZoo) {
		super();
		if (nCompl >= capMax) throw new IllegalArgumentException("Non ci pensare nemmeno");
		this.capMax = capMax;
		this.capNowBot = 0;
		this.capNowZoo = 0;
		this.nCompl = nCompl;
		this.pBot = pBot;
		this.pZoo = pZoo;
		this.ZooCond = lock.newCondition();
		this.BotCond = lock.newCondition();
		this.sospZoo = 0;
		this.sospBot = 0;
		this.totVisite = 0;
		this.totIncasso = 0;
	}
	
	private int getCapStatus() {
		return capNowBot + capNowZoo;
	}
	public void entraBot(String name) throws InterruptedException {
		lock.lock();
		try {
			while (getCapStatus() == capMax) {
				System.out.println("[B] Sono: "+name+" , capacita massima! aspetto..");
				sospBot++;
				BotCond.await();
				sospBot--;
			}
			System.out.println("[B] Sono: "+name+" entrato nella fattoria!!");
			capNowBot++;
			totVisite++;
			totIncasso += pBot;
		}finally {lock.unlock();}
	}
	
	public void esceBot() {
		lock.lock();
		try {
			
			capNowBot--;
			if (sospBot>0)
				BotCond.signal();
			else if (sospZoo>0)
				ZooCond.signal();
			
		}finally {lock.unlock();}
	}


	public void entraZoo(String name) throws InterruptedException {
		lock.lock();
		try {
			while (getCapStatus() == capMax || capNowZoo == nCompl) {
				System.out.println("[B] Sono: "+name+", "+
			(getCapStatus() == capMax? "capacita massima! ":"non cono completi!") 
			+ " aspetto..");
				sospZoo++;
				ZooCond.await();
				sospZoo--;
			}
			nCompl--;
			capNowZoo++;
			totVisite++;
			totIncasso += pZoo;
		}finally {lock.unlock();}
	}

	public void esceZoo() {
		lock.lock();
		
		capNowZoo--;
		nCompl++;
		
		if (sospZoo>0)
			ZooCond.signal();
		else if (sospZoo>0)
			BotCond.signal();
		
		lock.unlock();
	}

	
}
