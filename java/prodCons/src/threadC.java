public class threadC extends Thread { // consumatori
    int msg;
    Risorsa r;

    public threadC(Risorsa R) {
        this.r = R;
    }

    public void run() {
        System.out.print("\nThread CONSUMATORE: il mioID è: " + getName() + "..\n");
        while (true) {
            msg = r.prelievo();
            System.out.print("\n" + getName() + "consumatore ha letto il messaggio " + msg + "...\n");
        }
    }
}