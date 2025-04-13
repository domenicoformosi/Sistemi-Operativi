# scrivere un file comandi che prende in ingresso il nome di una directory e dopo aver controllato i parametri in ingresso
# controlla ogni 5 secondi se sono stati creati o cancellati file nella directory passata per argomento
# in caso di cambiamenti si stampa su stdout quanti file sono presenti nella directory
# n.b useremo un file temporaneo per tenere traccia mano a mano dei file

#!/bin/bash
if [ $# -ne 1 ] ; then echo Sintassi! ; exit; fi 
#controllo che sia stato passato uno e un solo parametro
if [ ! -d $1 ] ; then echo $1 non è una directory! ; exit; fi
#controllo che il file passato sia effettivamente una directory 
# -d controlla che l'argometno sia una directory, ! mi permette di esprimere negazione
echo 0 > loop.$$.tmp
#creo un file temporaneo per tenera traccia dei file
OK=0
while [ $OK -lt 10 ]
do
    new=`ls "$1"|wc -w` 
    # do in input a wc -w (conta le word) l'output di ls directory
    old=`cat loop.$$.tmp`
    # metto in old il numero precedente di file

    if [ $new -ne $old ] ;
    then echo $new > loop.$$.tmp ;
    echo in $1 ci sono $new file ;
    # se old e new non sono uguali stampo dentro il file temporaneo il nuovo numero di file e poi stampo su stdout
    fi
    OK=`expr $OK + 1` #incremento il mio contatore
    sleep 5s #nanna 5 secondi
done
rm /"$1"/*
rm loop.$$.tmp