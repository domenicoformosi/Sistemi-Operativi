package es10;


public class BambinoB extends Thread{
	Fattoria fattoria;

	public BambinoB(Fattoria f) {
		this.fattoria = f;
	}
	
	public void run() {
		try {
		fattoria.entraBot(getName());
		System.out.println("[B] Sono: "+getName()+" provo ad entrare nella fattoria!!");
			sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		fattoria.esceBot();
		System.out.println("[B] Sono: "+getName()+" uscito dalla fattoria!!");

	}
}
