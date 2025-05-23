package java.porto.src;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Monitor {
    public final static int IN = 0;
    public final static int OUT = 1;
    /*
     * Mi serve per identificare la direzione
     */

    private final static int BP = 0;
    private final static int BG = 1;
    /*
     * Identificano il tipo della barca (grande / piccolo)
     * 
     */

    private final static int std = 0;
    private final static int maxi = 1;
    /*
     * Indentificano il tipo di posto barca occupato (standard e maxi)
     * 
     * 
     */

    private int M; // numero posti maxi
    private int N; // numero posti normali
    private int Mocc; // maxi occupati
    private int Nocc; // standard occupati

    private Lock L = new ReentrantLock();

    private int[] piccoleC = new int[2];
    private int[] grandiC = new int[2];
    /*
     * Identificano numero di barche all'interno del canale (in IN e in OUT)
     * 
     */

    private int[] motovedettaC = new int[2];

    private Condition[] codaBP = new Condition[2];
    private Condition[] codaBG = new Condition[2];
    private Condition[] codaMV = new Condition[2];

    /*
     * Creo 2 code per tipo di veicolo (una per IN e una per OUT)
     * 
     */

    private int[] BPsosp = new int[2];
    private int[] BGsosp = new int[2];
    private int[] MVsosp = new int[2];

    /*
     * Conto i sospesi per ogni tipo di barca, anche quindi IN e OUT
     * 
     */

    public Monitor(int maxm, int maxn) {
        this.M = maxm;
        this.N = maxn;
        Mocc = 0;
        Nocc = 0;
        for (int i = 0; i < 2; i++) {
            this.piccoleC[i] = 0;
            this.grandiC[i] = 0;
            this.motovedettaC[i] = 0;
            this.codaBP[i] = L.newCondition();
            this.codaBG[i] = L.newCondition();
            this.codaMV[i] = L.newCondition();
            this.BPsosp[i] = 0;
            this.BGsosp[i] = 0;
            this.MVsosp[i] = 0;
        }
    }

    /*
     * Le regole di navigazione del canale sono le seguenti:
     * • La presenza di una motovedetta M in direzione D nel canale impedisce ad
     * ogni altra imbarcazione
     * di percorrere il canale in qualunque direzione (in questo caso, il canale
     * sarà quindi percorso da una
     * sola motovedetta alla volta.
     * • La presenza di una imbarcazione grande nel canale in direzione D impedisce
     * a qualunque altra
     * imbarcazione in direzione opposta a D di percorrere il canale (in questo
     * caso, quindi, il canale è
     * utilizzato a senso unico);
     * • Infine, se nel canale vi sono soltanto imbarcazioni piccole, esso potrà
     * essere utilizzato in entrambi i
     * versi di navigazione
     */
    public void entraMV(int dir) throws InterruptedException {
        L.lock();
        while (inCanale() != 0 ||
                (dir == IN && MVsosp[OUT] > 0)) {
            /*
             * Il controllo qua si basa sul numero di barche presenti nel canale (deve essere 0 per mv) e sull'unico
             * altro tipo a cui dobbiamo dare precedenza (se sono mv in in devo dare precedenza a mv in out)
             */
            this.MVsosp[dir]++;
            this.codaMV[dir].await();
            this.MVsosp[dir]--;
        }
        this.motovedettaC[dir]++;
        L.unlock();
    }

    public void esceMV(int dir) {
        L.lock();
        this.motovedettaC[dir]--;
        if (MVsosp[OUT] > 0)
            codaMV[OUT].signal();
        else if (MVsosp[IN] > 0) // risveglio motovedette IN
            codaMV[IN].signal();
        else { // risveglio barche in ordine di priorità
            if (BGsosp[OUT] > 0)
                codaBG[OUT].signalAll();
            if (BPsosp[OUT] > 0)
                codaBP[OUT].signalAll();
            if (BGsosp[IN] > 0)
                codaBG[IN].signalAll();
            if (BPsosp[IN] > 0)
                codaBP[IN].signalAll();
        }
        L.unlock();
    }

    public int entraB(int tipo, int dir, int posto) throws InterruptedException {
        int ris = -1;
        L.lock();
        if ((dir == IN) && (tipo == BP)) // barca piccola IN
        {
            while (motovedettaC[IN] + motovedettaC[OUT] > 0 ||
                    grandiC[OUT] > 0 ||
                    (Nocc == N && Mocc == M) ||
                    MVsosp[OUT] + MVsosp[IN] + BGsosp[OUT] + BPsosp[OUT] + BGsosp[IN] > 0) {
                        /*
                         *  prima parte di controllo: vedo se ci sono motovedette (in e out) nel canale
                         * poi controllo che non ci siano barche grandi (c'è ne puoi stare una piccola in out)
                         * controllo poi che ci sia posto (sia std che maxi)
                         * controllo che imbarcazioni con priorita elevata non siano in attesa
                         */
                BPsosp[IN]++;
                codaBP[IN].await();
                BPsosp[IN]--;
            }
            piccoleC[IN]++;
            if (Nocc < N) // occupa un posto nel porto
            {
                Nocc++;
                ris = std;
            } else {
                Mocc++;
                ris = maxi;
            }
            /*
             * So che il posto c'è devo solo capire se maxi o std per decrementarlo
             */
        }
        if ((dir == IN) && (tipo == BG)) // barca grande in direzione IN
        {
            while (motovedettaC[IN] + motovedettaC[OUT] > 0 ||
                    piccoleC[OUT] + grandiC[OUT] > 0 ||
                    Mocc == M ||
                    MVsosp[OUT] + MVsosp[IN] + BGsosp[OUT] + BPsosp[OUT] > 0) {
                        /*
                         * qui stessa roba di prima ma in piu abbiamo che non possono esserci nemmeno barche piccole in uscita
                         * n.b. perche non controllo anche piccoleC[in] ?? perche il metodo entra è mutualmente esclusivo e se ci sono
                         * io di certo non c'è qualcun altro, possiammo vedere che infine BG da precendeza solo a BP in out (non in)
                         */
                BGsosp[IN]++;
                codaBG[IN].await();
                BGsosp[IN]--;
            }
            grandiC[IN]++;
            Mocc++; // occupa un posto nel porto
            ris = maxi;
        }
        if ((dir == OUT) && (tipo == BP)) // barca piccola OUT
        {
            while (motovedettaC[IN] + motovedettaC[OUT] > 0 ||
                    grandiC[IN] > 0 ||
                    MVsosp[OUT] + MVsosp[IN] + BGsosp[OUT] > 0) {
                BPsosp[OUT]++;
                codaBP[OUT].await();
                BPsosp[OUT]--;
            }
            piccoleC[OUT]++; // libera il posto
            if (posto == std)
                Nocc--;
            else
                Mocc--;
            // sveglio una piccola IN:
            if (BPsosp[IN] > 0 && grandiC[OUT] == 0)
                codaBP[IN].signal();

        }
        if ((dir == OUT) && (tipo == BG)) // barca grande OUT
        {
            while (motovedettaC[IN] + motovedettaC[OUT] > 0 ||
                    piccoleC[IN] + grandiC[IN] > 0 ||
                    MVsosp[OUT] + MVsosp[IN] > 0) {
                BGsosp[OUT]++;
                codaBG[OUT].await();
                BGsosp[OUT]--;
            }
            grandiC[OUT]++;
            Mocc--;
            // non segnalo nessuno IN perchè non potrebbe entrare
        }
        L.unlock();
        return ris;
    }

    public void esceB(int tipo, int dir) {
        L.lock();
        if (tipo == BG)
            grandiC[dir]--;
        else
            piccoleC[dir]--;
        if (MVsosp[OUT] > 0 && inCanale() == 0)
            codaMV[OUT].signal();
        else if (MVsosp[IN] > 0 && inCanale() == 0)
            codaMV[IN].signal();
        else if (dir == OUT) {
            if (BGsosp[IN] > 0 && grandiC[OUT] + piccoleC[OUT] == 0 && Mocc < M)
                codaBG[IN].signalAll();
            if (BPsosp[IN] > 0 && grandiC[OUT] == 0 && Mocc + Nocc < M + N)
                codaBP[IN].signalAll();
        } else if (dir == IN)// dir IN
        {
            if (BGsosp[OUT] > 0 && grandiC[IN] + piccoleC[IN] == 0)
                codaBG[OUT].signalAll();
            if (BPsosp[OUT] > 0 && grandiC[IN] == 0)
                codaBP[OUT].signalAll();
        }
    }

    private int inCanale() {
        return piccoleC[IN] + piccoleC[OUT] + grandiC[IN] + grandiC[OUT] +
                motovedettaC[IN] + motovedettaC[OUT];
    }
}