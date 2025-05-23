package es10_3;

public class BambinoZ extends Thread{
	Fattoria fattoria;
	
	public BambinoZ(Fattoria f) {
		this.fattoria = f;
	}
	
	public void run() {
		try {
		System.out.println("[Z] Sono: "+getName()+" provo ad entrare nella fattoria!!");
		fattoria.entraZoo(getName());

			sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		fattoria.esceZoo();
		System.out.println("[Z] Sono: "+getName()+" uscito dalla fattoria!!");

	}
}