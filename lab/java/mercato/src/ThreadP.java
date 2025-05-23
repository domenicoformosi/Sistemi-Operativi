public class ThreadP extends Thread {
	Risorsa r;

    public ThreadP(Risorsa r) {
        this.r = r;
    }
    public void run() {
        int Qp;
        Qp = (int) (Math.random() *10+1);
        System.out.println("[P]: mio ID: "+getName()+ " e voglio vendere "+Qp+" quintali..");
        r.vendi(Qp);
    }
}
