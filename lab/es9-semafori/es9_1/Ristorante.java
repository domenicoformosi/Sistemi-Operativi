package es9_1;

import es9_2.Tavolo_2;

public class Ristorante {
	public static void main(String[] args) {
	Tavolo_2 t = new Tavolo_2(20,20);
	AddettoLavaggio lavastoviglie = new AddettoLavaggio(t,2,2);
	CameriereB bicchieri = new CameriereB(t);
	CameriereP piatti = new CameriereP(t);
	
	lavastoviglie.start();
	bicchieri.start();
	piatti.start();
	
	}
}
