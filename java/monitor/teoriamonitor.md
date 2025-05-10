# Un po di teoria sui monitor ( con esercizi )

## Introduction
Con la nascita dei sistemi multiprogrammati nasce chiaramente la necessita di avere strumenti che mi garantiscano sincronizzazione e mutua esclusione delle risorse, abbiamo gia visto i semafori ma facendo due conti ci si rende conto che sono un pò troppo a basso livello e che sono la scelta migliore nei contesti in cui le performance sono più importanti, ma sò effetivamente un po scomodi

## Table of Contents
- [Definizione](#Definizione)
- [Doppio livello di sincronizzazione](#doppio-livello-di-sincronizzazione)
    - [Mutua esclusione](#mutua-esclusione)
    - [Ordine](#ordine)
- [Conclusion](#conclusion)

## Definizione
Sono un costrutto sintattico che associa un insieme di operazione ( **entry** ), ad una struttura dati comune a più processi, e fino a qua, voi tutti direte *e che cambia rispetto ad una semplice classe?*, la differenza sta nel fatto che il monitor è definito ( e implementantato ) in maniera tale da:
    - Rendere le **entry** le uniche operazioni possibili sulla struttura dati condivisa
    - Rendere le **entry** mutualmente esclusive 

``` java
    monitor tipoRisorsa {
        <Dichiarazione e inizializzazione var. locali>
        
        entry void op1 (){

        }

        entry void op2(){

        }

        <Altre operazioni non entry>
    }
```
Le variabili locali sono accessibili sono all'interno del monitor, le operazioni entry sono le uniche che possono essere utilizzate dall'esterno ( in modo mutualmente esclusivo ), le variabili locali sono permanenti, ovvero mantengono il loro valore tra diverse esecuzioni delle operazioni del monitor


## Doppio livello di sincronizzazione
### Mutua esclusione
Il primo livello di sincronizzazione è quello che garantisce che solo un processo alla volta acceda al monitor, e quindi alla risorsa. E' possibile sospensione del processo e inserimento nella **entry queue**, questo primo livello corrisponde all'utilizzo del semaforo *mutex* nelle slide precendenti
### Ordine
Il secondo livello va a controllare l'**ordine** con il quale i processi hanno accesso alla risorsa,
La procedura chiamata verifica il soddisfacimento della **condizione di sincronizzazione** che mi garantisce l'ordinamento. Se la condizione non è rispettata il processo va in attesa
> **⚠️ Attenzione:** L'attesa del processo non è nella entry queue perche quella è per l'accesso al monitor (e quindi la risorsa)

Se per il primo livello se lo smazza tutto il linguaggio con i suoi costrutti, l'implementazione dell'ordine tocca a noi programmatori tramite un nuovo strumento detto **condition**,
questa variabile **condizione** non rappresenta altro che una coda nella quale i processi possono sospendersi (se non condizione non verificata, sospensione avviene chiaramente andando a rilasciare la risorsa monitor in maniera automatica)

Le operazioni che si possono applicare alla variabile ` condition cond ` sono due
 - wait(cond)
 - signal(cond)

L'esecuzione della `wait` mi va a sospendere il processo andandolo a mettere nella coda associata alla variabile cond, e il monitor viene successivamente liberato, al risveglio, che è possibile mendiante `signal` il processo riacquisisce la risorsa
> **💡 N.B.:** Quando un processo viene risvegliato tramite `signal(cond)`, non riprende immediatamente l'esecuzione. Deve attendere che il monitor sia nuovamente disponibile, rispettando il primo livello di sincronizzazione (mutua esclusione). Questo comportamento è fondamentale per evitare situazioni di race condition.

> **💡 N.B.:** Quello che succede nella wait quando avevamo i semafori dovevamo farcelo a mano, se dovevamo sospendere un processo doveva assicurarci di far rilasciare la risorsa a quest'ultimo

``` java
    monitor Risorsa(){
        boolean risorsaIsLibera = true;
        condition C;
        int turno; // definisce ordinde di accesso alla risorsa

        entry void acquisizione (int id){
            while (turno !=  id || !risorsaIsLibera){
                C.wait();
            }
            risorsaIsLibera = false;
        }

        entry void rilascio (int id){

            risorsaIsLibera = true;
            <qui avrò una politica per assegnare un nuovo valore a turno>

            C.signal();
        }

    }
```
# Conclusion
Summarize the key points or provide closing remarks.

---
*Generated on: [Insert Date Here]*b