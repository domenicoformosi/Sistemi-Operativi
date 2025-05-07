package es8_thread;

import java.util.Random;

public class Main2 {
	private static Random spesa = new Random();
	public static void main(String[] args) throws InterruptedException {
		
		Museo m = new Museo("Louvre",10,10,10);
		Visitatore2 v1 = new Visitatore2("123",true,spesa,m);
		Visitatore2 v2 = new Visitatore2("124",true,spesa,m);
		Visitatore2 v3 = new Visitatore2("125",false,spesa,m);
		Visitatore2 v4 = new Visitatore2("126",false,spesa,m);
		Visitatore2 v5 = new Visitatore2("127",true,spesa,m);
		Visitatore2 v6 = new Visitatore2("128",true,spesa,m);
		Visitatore2 v7 = new Visitatore2("129",true,spesa,m);
		Visitatore2 v8 = new Visitatore2("130",true,spesa,m);
		Visitatore2 v9 = new Visitatore2("131",true,spesa,m);
		Visitatore2 v10 = new Visitatore2("132",true,spesa,m);
		Visitatore2 v11 = new Visitatore2("133",true,spesa,m);
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
