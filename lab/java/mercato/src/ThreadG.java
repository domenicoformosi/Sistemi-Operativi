public class ThreadG extends Thread {
    Risorsa r;
    
    public ThreadG(Risorsa r) {
        this.r = r;
    }
    
    public void run() {
        int Qg;
        Qg = (int) (Math.random() *10+1);
        System.out.println("[G]: mio ID: "+getName()+ " e voglio comprare "+Qg+" quintali..");
        r.compra(Qg);
    }
    
}
