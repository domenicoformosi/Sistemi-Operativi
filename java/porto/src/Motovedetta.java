package java.porto.src;
import java.util.Random;
public class Motovedetta {

    Monitor porto;
    Random rnd;
    String id;
    private int IN;
    private int OUT;

    public Motovedetta(Monitor porto,Random rnd, String id){
        this.porto = porto;
        this.rnd = rnd;
        this.id = id;
        IN = porto.IN;
        OUT = porto.OUT;
    }

    public void run(){
        try{
        porto.entraMV(OUT);

        Thread.sleep(rnd.nextInt(100),OUT);
            porto.esceMV(IN);
        Thread.sleep(rnd.nextInt(1000));

        porto.entraMV(OUT);
        

        Thread.sleep(rnd.nextInt(100));

        porto.esceMV(OUT);
        }catch(InterruptedException e){
            //suca
        }
    }
}