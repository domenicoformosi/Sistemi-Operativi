#!/bin/bash

#prendo in input fout, N e n nomi assoluti di directory esistenti
N=$2
fout=$1


#controlli iniziali:

# - ho passato almeno 3 argomenti
if [[ $# -lt 3 ]] ; then echo Sintassi ! mi hai passato troppi pochi argomenti !! ; exit ; fi

# - N deve essere un intero positivo
if [[ $2 -le 0 ]] ; then echo Sintassi ! mi hai passato un valore \<= 0 !! ; exit ; fi

# -fout deve essere un pathname assoluto non deve esistere
if [[ $fout != /* ]] ; then echo Sintassi ! fout non è un pathname assoluto!! ; exit ; fi
if [[ -f $fout ]] ; then echo Sintassi ! fout esiste già !! ; exit ; fi
> $fout #creo fout

# - devo controllare che tutti le n directory siano assolute e esistenti
# posso shiftare di due posizioni ( mi perdo N e fout e ciclo su parametri passati in ingresso ( magari me li salvo prima) )
shift
shift
# mo dovrei stare apposto e posso ciclare

count=0
for i ;
do
    count=`expr $count + 1`
    if [[ ! -d "$i" ]]; then echo Il parametro "$i" non è una directory, o perlomeno non esiste; exit; fi
    if [[ "$i" != /* ]] ; then echo La directory "$i" non è un pathname assoluto; exit; fi
done

# -check finale non richiesto, controllo che il numero di directory passate ed N sia effettivamente uguale

if [[ $N -ne $count ]] ; then echo Non fare il furbetto !; exit; fi

# okay adesso devo ispezionale il contenuto di tutte le directory
# per ogni directory considero solo i file leggibili ( -r  mi dice se ho diritto di lettura )
# per ogni file conto il numero di parole nelle ultime 10 righe ( come ?? )
# faccio somma di tutti questi valori in una variabile 

count=0 #riciclo variabile count per sommare parole 
for dir ;
do 

    # ciclo prima si tutta le directory
    for f in `ls $dir` ;
    do 

        cd $dir
        # qua devo contare parole nelle ultime 10 righe
        numParole=`tail $f | wc -w`
        count=`expr $count + $numParole`
    done

    if [[ $count -gt $N ]] ; then
    echo  "$dir $count" >> $fout
    fi
done

# quando esco dal for ho in fout tutte le directory con num di parole, posso filtrare con awk e poi fare classica ricerca del max

# prima di tutto capisco quante righe ha il mio file

# posso fare o cosi

#numRighe=`wc -l $fout | cut -d " " -f 1`

#oppure

#numRighe=`wc -l fout | awk '{ print $1 }'`

# a quanto pare ottenere il numero di righe è inutile perche il for mi permette di considerare direttamente le righe, lascio perchè è utile per capire come funzioan cut e awk
max=0
for i in `awk '{ print $2 }' $fout`
do 
    if [[ $i -gt $max ]]; then
        max=$i

    fi
done


echo Directory con count più grande: `grep $max $fout | awk '{print $1}'`

rm $fout