import java.util.concurrent.Semaphore;

public class Risorsa {
    final int PV=10; // prezzo di vendita
    final int PA=12; // prezzo di acquisto
    final int MAX=100; // capacita massima

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
        /* dopo si capisce perche questi sono inzialmente 0 */
        sM = new Semaphore(1);
        this.merce = merce;
        this.cassa = cassa;

        sospesiG = 0;
        sospesiP = 0;

    }

    public void compra (int quintali){
        var prezzo = quintali * PA;
        try{
            sM.acquire(); // inizio sez. critica su risorsa
            while (quintali > merce) {
                sospesiG++;

                sM.release();
                sG.acquire();
                /*  se non c'è abbastanza merce mediante sG mi metto in attesa? quando è che mi libero? quando un produttore inserisce della merce farà una release! */
                sM.acquire();
                /* chiaramente quando il grossista aspetta non ha senso tenere occupata la risorsa mercato */
            }
            merce -= quintali;
            cassa += prezzo;

            while (sospesiP > 0){
                sP.release();
                sospesiP--;
                /* alla stessa maniera qui quando vendo merce rilascio il semaforo dei produttori, ho liberato spazio e generato cassa */
            }
            sM.release();

        }catch(InterruptedException e){}
    }

    public void vendi(int quintali){
        int ricavo = quintali * PV;
        try{    
            sM.acquire();
            while ((ricavo>cassa) || (quintali+merce>MAX)){
                /*  condizione essenzialmente mi dice finche non ci sono soldi o non c'è spazio, in maniera duale qua andro a rilasciare la risorsa condivisa,
                 incrementare produttori sopesi e metterli in attesa, abbiamo visto sopra che ogni volta che qualcuno compra c'è una release */
                 sospesiP++;
                 sM.release();
                 sP.acquire();
                 sM.acquire();
            }
            merce += quintali;
            cassa -= ricavo;
            while (sospesiG > 0){
                sG.release();
                sospesiG--;
                /* quando produttori mi lasciano qualcosa rilascio grossisti */
            }
            sM.release();
        }catch(InterruptedException e){};
    }

    
}
