# Esercizio Mercato (programmazione concorrente in java)


## Specifiche
Si vuole realizzare un sistema per la gestione delle transazioni commerciali relative a un mercato
ortofrutticolo all’ingrosso.
Il sistema deve permettere a un numero arbitrario di thread concorrenti, ciascuno rappresentante un
utente, di effettuare acquisti (nel caso di grossisti) e vendite (nel caso di produttori) .
Per semplicità si supponga che:
- il mercato tratti solo un tipo di merce (es. ciliegie), che può essere acquistato/venduto a multipli
interi del quintale
- il prezzo di vendita al quintale (PV) e di acquisto al quintale (PA) siano costanti.

Il mercato ha una capacità massima M che esprime il numero massimo di quintali di merce che possono
essere immagazzinati.
Gli utenti sono quindi di due tipi:
- Produttore: vende al mercato un quantitativo Qp di merce al prezzo PV; ottiene quindi la somma Qp*PV
- Grossista: acquista dal mercato un quantitativo Qg di merce al prezzo PA; spende quindi la somma Qg*PA
Si sviluppi un’applicazione java che, rappresentando ogni utente come un thread concorrente e
utilizzando i semafori come strumenti di sincronizzazione tra thead, realizzi il sistema



## Aproccio risolutivo
### Classe risorsa
Identifichiamo quindi il mercato come la nostra risorsa condivisa tha thread concorrenti (grossisti e produttori), le variabili caratterizzanti della nostra risorsa sono quindi:
- **Merce** : quantita di merce (in quintali) presente nel mercato
- **Cassa** : somma in euro disponibile nella cassa del mercato

### Due tipi di thread
Avremo quindi:
- **Produttore**: immette merce ed estrae soldi
- **Consumatore**: immette soldi ed estrae merce


### Sincronizzazione: 3 semafori
- **sM**: semaforo di mutua esclusione per accesso alla risorsa mercato
- **sP**: semaforo per sospendere produttori
-**sG**: semaforo per sospendere grossisti

#### Nota:
Come è possibile vedere nelle slide (pacchetto 11), **sM** serve per non accedere in maniera scorretta alla risorsa condivisa, ma quindi, a che servono gli altri due semafori?? I semafori possono (al giorno d'oggi devono) essere usati per far comunicare thread e implementare quindi sincronizzazione, quindi **sP** e **sG** sono i cosidetti **semafori risorsa**