package es8_thread;

import java.util.Random;

public class Visitatore extends Thread {
	Museo m;
	private Random spesa;
	private boolean ingressoShopping;
	private float spesaMinima = (float) 4.9;
	private float spesaMassima = (float) 100.0;
	
	public Visitatore(String ID,boolean shopping,Random spesa,Museo m) {
		super(ID);
		this.ingressoShopping = shopping;
		this.spesa = spesa;
		this.m = m;
	}
	
	public void run() {
		boolean entrato = m.entraMostra();
		if (entrato) {
			
			try {
				sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			m.esciMostra();
			
			if (ingressoShopping) {
				m.entraShopping();
				/*
				 * 
				 * 
				 * qua devo fare qualcosa??
				 * 
				 * 
				 */
				m.esciShopping(spesa.nextFloat(spesaMinima,spesaMassima));
			}
		}else {
			System.out.println("Accesso alla mostra non riuscito\n");
		}
	}
}
