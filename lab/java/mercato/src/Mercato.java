public class Mercato {
    public static void main(String args[]){
        int i;
        final int NP=10;
        final int NG=10;
        final int MERCE=9;
        final int CASSA=50;
        Risorsa r = new Risorsa(MERCE, CASSA);

        ThreadP []TP = new ThreadP[NP];
        ThreadG []TG = new ThreadG[NG];

        for (i=0; i <NP; i++) TP[i] = new ThreadP(r);
        for (i=0; i <NG; i++) TG[i] = new ThreadG(r);
        for (i=0; i <NP; i++) TP[i].start();
        for (i=0; i <NG; i++) TG[i].start();
    }
}
