package es9_1;

import es9_2.Tavolo_2;

public class CameriereP extends Thread{
	
	Tavolo_2 t;
	
	public CameriereP(Tavolo_2 t) {
		this.t = t;
	}
	
	public void run() {
		
		while (true) {
			System.out.println("[P]: mio ID: "+getName()+ " sto provando a lasciare un piatto");
			try {
				t.depositaPiatto(getName());
				sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			
		}
	}

}
