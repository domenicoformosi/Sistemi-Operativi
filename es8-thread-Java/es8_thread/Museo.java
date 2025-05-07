
package es8_thread;

public class Museo {
	private String nome;
	private int visMostra;
	private int visShopping;
	private int capienzaMaxMuseo;
	private int capienzaMaxMostra;
	private int prezzoMostra;
	private int incassiMostra;
	private int incassiShopping;
	private int ingressiTotaliMostra;
	private int ingressiTotaliShopping;
	
	
	
	public Museo(String nome, int capMo, int capTot, int prezzoMostra) {
		if (capMo>capTot) throw new IllegalArgumentException("Capienza massima < capienza mostra\n");
		if (prezzoMostra<0) throw new IllegalArgumentException("Prezzo ticket insensato\n");
		this.capienzaMaxMuseo = capTot;
		this.capienzaMaxMostra = capMo;
		this.prezzoMostra = prezzoMostra;
		this.incassiMostra = 0;
		this.incassiShopping = 0 ;
		this.visShopping = 0;
		this.visMostra = 0;
		this.ingressiTotaliMostra = 0;
		this.ingressiTotaliShopping = 0;
	}
	
	public synchronized int getVisitatoriAttuali() {
		return visMostra + visShopping;
	}
	public synchronized int getIncassiTotali(){
		return incassiMostra + incassiShopping;
	}
	
	public synchronized void print() {
		System.out.println("Ingressi totali mostra: "+ingressiTotaliMostra+
				"\nIngressi totali area shopping "+ingressiTotaliShopping+
				"\nIncassi totali: "+getIncassiTotali());
	}
	
	public synchronized boolean entraMostra() {
		boolean entrato;
		
		if ((visMostra >= capienzaMaxMostra) || (getVisitatoriAttuali() >= capienzaMaxMuseo)) entrato = false;
		else {
			entrato = true;
			visMostra++;
			incassiMostra+=this.prezzoMostra;
			ingressiTotaliMostra++;
			}
				
		return entrato;
	}
	
	public synchronized void esciMostra() {
		visMostra--;
	}
		
	public synchronized void entraShopping() {
		visShopping++;
		ingressiTotaliShopping++;
		
	}
	
	public synchronized void esciShopping(float spesa) {
		visShopping--;
		incassiShopping+=spesa;
	}

	public String getNome() {
		return nome;
	}

	
}
