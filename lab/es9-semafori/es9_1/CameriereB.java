package es9_1;

import es9_2.Tavolo_2;

public class CameriereB extends Thread {
	Tavolo_2 t;
	
	public CameriereB(Tavolo_2 t) {
		this.t = t;
	}
	
	public void run() {
		
		while (true) {
			System.out.println("[B]: mio ID: "+getName()+ " sto provando a lasciare un bicchiere");
			try {
				t.depositaBicchiere(getName());
				sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			
		}
	}
}
