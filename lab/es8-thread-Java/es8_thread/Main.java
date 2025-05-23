package es8_thread;

import java.util.Random;

public class Main {
	private static Random spesa = new Random();
	public static void main(String[] args) throws InterruptedException {
		
		Museo m = new Museo("Louvre",10,10,10);
		Visitatore v1 = new Visitatore("123",true,spesa,m);
		Visitatore v2 = new Visitatore("124",true,spesa,m);
		Visitatore v3 = new Visitatore("125",false,spesa,m);
		Visitatore v4 = new Visitatore("126",false,spesa,m);
		Visitatore v5 = new Visitatore("127",true,spesa,m);
		Visitatore v6 = new Visitatore("128",true,spesa,m);
		Visitatore v7 = new Visitatore("129",true,spesa,m);
		Visitatore v8 = new Visitatore("130",true,spesa,m);
		Visitatore v9 = new Visitatore("131",true,spesa,m);
		Visitatore v10 = new Visitatore("132",true,spesa,m);
		Visitatore v11 = new Visitatore("133",true,spesa,m);
		v1.start();
		v2.start();
		v3.start();
		v4.start();
		v5.start();
		v6.start();
		v7.start();
		v8.start();
		v9.start();
		v10.start();
		v11.start();

		/*
		 * creazione visitatori
		 * attivazione thread
		 * attesa terminazione
		 */
		v1.join();
		v2.join();
		v3.join();
		v4.join();
		v5.join();
		v6.join();
		v7.join();
		v8.join();
		v9.join();
		v10.join();
		v11.join();
		
		
		
		m.print();
		
	}
}
