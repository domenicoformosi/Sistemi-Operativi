import java.util.concurrent.Semaphore;

public class Risorsa {
    // definizione buffer condiviso:
    final int N = 30; // capacità del buffer
    int lettura, scrittura;// indice di lettura
    int[] buffer;
    Semaphore sP; /* sospensione dei Produttori; v.i. N */
    Semaphore sC; /* sospensione dei Consumatori v.i. 0 */
    Semaphore sM1, sM2; // semafori di mutua esclus. v.i. 1

    public Risorsa() // costruttore
    {
        lettura = 0;
        scrittura = 0;
        buffer = new int[N];
        sP = new Semaphore(N); /* v.i. N */
        sC = new Semaphore(0); /* v.i. 0 */
        sM1 = new Semaphore(1);
        sM2 = new Semaphore(1);
    } // continua..

    public void inserimento(int M) {
        try {
            sP.acquire();
            sM1.acquire(); // inizio sez critica
            buffer[scrittura] = M;
            scrittura = (scrittura + 1) % N;
            sM1.release(); // fine sez critica
            sC.release();
        } catch (InterruptedException e) {
        }
    }

    public int prelievo() {
        int messaggio = -1;
        try {
            sC.acquire();
            sM2.acquire(); // inizio sez critica
            messaggio = buffer[lettura];
            lettura = (lettura + 1) % N;
            sM2.release(); // fine sez critica
            sP.release();
        } catch (InterruptedException e) {
        }
        return messaggio;
    }

    public static void main(String[] args) {
        Risorsa risorsa = new Risorsa();
        System.out.println("Test della Risorsa");
        // Aggiungi qui il codice per testare i metodi inserimento e prelievo
    }
}