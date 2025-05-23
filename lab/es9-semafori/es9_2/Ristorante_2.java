package es9_2;

public class Ristorante_2 {
	public static void main(String[] args) {
	Tavolo_2 t = new Tavolo_2(50,20,20);
	AddettoLavaggio_2 lavastoviglie = new AddettoLavaggio_2(t,2,2);
	CameriereB_2 bicchieri = new CameriereB_2(t);
	CameriereP_2 piatti = new CameriereP_2(t);
	
	lavastoviglie.start();
	bicchieri.start();
	piatti.start();
	
	}
}
