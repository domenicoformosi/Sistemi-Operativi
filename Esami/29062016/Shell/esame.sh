#!/bin/bash
# assumo dirin e dirout siano presenti nello stesso folder di esecuzione dello script
# altrimenti non funziona
# non fosse cosi dovrei accertarmi che mi vengano passati come path assoluti
# o comunque dovrei generare io il path

# dopo averci pensato meglio usare path assoluti, visto che non sapevo minimanete
# come fare ho cercato su internet e posso usare realpath per farmi fornire path
# assoluto
dirin=`(realpath $1)`
dirout=`(realpath $4)`
echo $dirin $dirout
if [[ ! -d $dirin ]]; then echo Sintassi! $dirin non è una direcotry; exit; fi
if [[ ! -d $dirout ]]; then echo Sintassi! $dirout non è una direcotry; exit; fi
if [[ $2 =~ ^[0-9]+$ ]]; then echo Sintassi! $2 contiene dei numeri; exit; fi
if [[ $3 -le 0 ]]; then echo Sintassi! $3 non è un intero positivo ! ; exit ; fi

if [[ "$0" = /* ]] ; then
    dirName=`dirname "$0"`
    recursiveScript="$dirName/recursive.sh"
elif [[ "$0" = */* ]] ; then
    dirName=`dirname "$0"`
    echo $dirName
    recursiveScript="$PWD/$dirName/recursive.sh"
    echo a
else 
    recursiveScript="recursive.sh"
fi
echo $recursiveScript
# il file ricorsivo deve copiare dalla directory corrente (nel sottoalbero di dirin)
# in dirout, mi conviene generare path assoluto anche di dirout (sempre assumendo stia in questo folder)

# problema sopra risolto usando realpath, lo lascio perche si

"recursiveScript" "$dirin" "$2" "$3" "$dirout"

# note finali: la struttura della soluzione è simile alla soluzione
# mi fa molto piacere (sono ironico) che nella soluzione non si sono fatti pare per path assoluti/relativ di dirin e dirout
