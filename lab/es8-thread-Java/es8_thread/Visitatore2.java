package es8_thread;

import java.util.Random;

public class Visitatore2 extends Thread {
	Museo m;
	private Random spesa;
	private boolean ingressoShopping;
	private float spesaMinima = (float) 4.9;
	private float spesaMassima = (float) 100.0;
	
	public Visitatore2(String ID,boolean shopping,Random spesa,Museo m) {
		super(ID);
		this.ingressoShopping = shopping;
		this.spesa = spesa;
		this.m = m;
	}
	
	public void run() {
		boolean entrato = m.entraMostra();
		
		while (!entrato) {
			System.out.println("Non sono riuscito ad entare! aspetto:(");
			entrato = m.entraMostra();
			try {
				sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		System.out.println("Sono entrato! mi faccio un giro;)");
		try {
			sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
			m.esciMostra();
			
			if (ingressoShopping) {
				m.entraShopping();

				m.esciShopping(spesa.nextFloat(spesaMinima,spesaMassima));
			}

	}
}
