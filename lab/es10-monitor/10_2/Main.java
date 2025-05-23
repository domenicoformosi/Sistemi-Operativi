package es10_2;

public class Main {
	/* main esercizio 2*/
	public static void main (String args[]) throws InterruptedException {
		Fattoria f = new Fattoria(10,5,10,15);
		/* capMax, nCompl, pBot, pZoo */
		
		BambinoB[] bambiniB = new BambinoB[100] ;
		BambinoZ[] bambiniZ = new BambinoZ[10] ;
		
		for (int i=0;i<bambiniB.length;i++) {
			bambiniB[i] = new BambinoB(f);
		}
		for (int i=0;i<bambiniZ.length;i++) {
			bambiniZ[i] = new BambinoZ(f);
		}
		for (int i=0;i<bambiniB.length;i++) {
			bambiniB[i].start();
		}
		for (int i=0;i<bambiniZ.length;i++) {
			bambiniZ[i].start();
		}
		for (int i=0;i<bambiniB.length;i++) {
			bambiniB[i].join();;
		}
		for (int i=0;i<bambiniZ.length;i++) {
			bambiniZ[i].join();
		}
		
		
		System.out.println(f.toString());
		
	}
}
