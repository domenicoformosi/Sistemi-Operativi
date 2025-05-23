package es9_1;
import java.util.concurrent.Semaphore;

public class Tavolo {
	
	private int maxP;
	private int maxB;
	private int nP;
	private int nB;
	private int sospesoCP,sospesoCB,sospesoL; // sospeso per camerieri e addetto lavastoviglie
	private Semaphore mutex; // semaforo mutua esclusione
	private Semaphore sP; // semaforo privato piatti
	private Semaphore sB; // semaforo privato bicchieri
	private Semaphore sL; // semaforo privato lavastoviglie
	
	public Tavolo(int maxP, int maxB) {
		
		if (maxP <= 0 || maxP <= 0) throw new IllegalArgumentException("Capacità illegalie");
		this.maxB = maxB;
		this.maxP = maxP;
		
		this.nP = 0;
		this.nB = 0;
		
		this.sospesoCP= 0;
		this.sospesoCB= 0;
		this.sospesoL = 0;
		
		this.mutex = new Semaphore(1);
		
		this.sP = new Semaphore (0);
		this.sB = new Semaphore (0);
		this.sL = new Semaphore (0);
	}
	
	public void depositaPiatto(String name) throws InterruptedException {
		mutex.acquire();
		while (nP == maxP) {
			
			mutex.release();
			sospesoCP++;
			sP.acquire();
			mutex.release();
			/*
			 *  rilascio risorsa --> mi metto in attesa quando addetto stoviglie
			 * toglie robe dal tavolo faccio release e libero spazio
			 * 
			 * */
			
		}
		System.out.println("[P]: mio ID: "+name+ " sto provando a lasciare un piatto");
		nP++;
		while (sospesoL>0) { 
			sL.release();
			sospesoL--;
		}
		mutex.release();
		
	}
	public void depositaBicchiere(String name) throws InterruptedException {
		mutex.acquire();
		while (nB == maxB) {
			
			mutex.release();
			sospesoCB++;
			sB.acquire();
			mutex.release();
			/*
			 *  rilascio risorsa --> mi metto in attesa quando addetto stoviglie
			 * toglie robe dal tavolo faccio release e libero spazio
			 * 
			 * */
			
		}
		System.out.println("[B]: mio ID: "+name+ " ho lasciato un bicchiere");
		nB++;
		while (sospesoL>0) { 
			sL.release();
			sospesoL--;
		}
		mutex.release();
	}
	
	public void prelevaPerLavastoviglie(String name,int numBla, int numPla) throws InterruptedException {
		mutex.acquire();
		while (nP<numPla || nB < numBla) {
			// finche non ho abbastanza stoviglie per riempire lavastoviglie
			mutex.release();
			sospesoL++;
			sL.acquire();
			mutex.acquire();
		}
		System.out.println("[L]: mio ID: "+name+ " caricata lavastoviglie");
		nP -= numPla;
		nB -= numBla;
		
		while (sospesoCP>0) {
			sP.release();
			sospesoCP--;
		}
		while (sospesoCB>0) {
			sB.release();
			sospesoCB--;
		}
		mutex.release();
		
		
	}
	

}
