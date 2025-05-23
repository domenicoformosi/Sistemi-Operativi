package es9_1;

import es9_2.Tavolo_2;

public class AddettoLavaggio extends Thread {
	private Tavolo_2 t;
	private int maxP;
	private int maxB;
	
	public AddettoLavaggio(Tavolo_2 t, int maxP, int maxB) {
		this.t = t;
		this.maxB = maxB;
		this.maxP = maxP;
	}
	
	public void run() {
		while (true) {
		System.out.println("[L]: mio ID: "+getName()+ " sto provando a caricare lavastoviglie");
		try {
			t.prelevaPerLavastoviglie(getName(),maxP,maxB);
			sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		}
		
	}
}
