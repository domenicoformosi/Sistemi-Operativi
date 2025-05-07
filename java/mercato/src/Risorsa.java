import java.util.concurrent.Semaphore;

public class Risorsa {
    final int PV=10; // prezzo di vendita
    final int PA=12; // prezzo di acquisto
    final int NAX=100; // capacita massima

    int merce,cassa; //variabili caratterizzanti

    Semaphore sP;
    Semaphore sG;
    /* semafori risorsa */
    int sospesiP; 
    /* Numero di produttori sospoesi ==> stanno aspettando o i soldi o spazio */

    int sospesiG;
    /* Grossisti sospesi ==> stanno aspettando la merce */

    Semaphore sM; // mutua esclusione

    
    public Risorsa (int merce,int cassa){
        sP = new Semaphore(0);
        sG = new Semaphore(0);

        sM = new Semaphore(1);
        this.merce = merce;
        this.cassa = cassa;

        sospesiG = 0;
        sospesiP = 0;

    }

    public void compra (int quintali){
        int i;
        var prezzo = quintali * PA;
        try{
            sM.acquire(); // inizio sez. critica su risorsa
            while (quintali > merce) {
                sospesiG++;
                sM.release();
                sG.acquire();
                sM.acquire();

            }
        }

    }

    
}
