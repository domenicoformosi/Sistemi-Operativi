#!/bin/bash

if [[ ! -d "$1" ]]; then echo "$1 non è una directory!";exit;fi
if [[ "$1" != /* ]]; then echo "$1 non è assoluta!"; exit ; fi
if [[ "$3" != /* ]]; then echo "$3 non è assoluta!"; exit ; fi

if [[ $2 -le 0 ]]; then echo "Dovevi darmi interi positivo..";exit; fi

if [[ -d "$3" ]]
then
    echo "flatDir esiste cancello tutto"
    
    for i in `ls "$3"`
    do
        rm -r "$3"/"$i"
    done
else
    echo "creo flatDir"
    mkdir "$3"
fi

if [[ "$0" = /* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/rec.sh"
elif [[ "$0" = */* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/rec.sh"
else
    recursive_cmd=rec.sh
fi
echo a
echo $recursive_cmd


if [[ "$0" = /* ]] ; then
    #Iniziando con /, si tratta di un path assoluto
    #(eg /home/andrea/recurse_dir.sh)

    #Estrazione di parti di path: man dirname oppure man basename
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/rec.sh"
elif [[ "$0" = */* ]] ; then
    # C'è uno slash nel comando, ma non inizia con /. Path relativo
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/rec.sh"
else 
    # Non si tratta ne di un path relativo, ne di uno assoluto.
    # E' un path "secco": il comando sarà dunque cercato
    # nelle cartelle indicate dalla variabile d'ambiente $PATH.
    recursive_cmd=rec.sh
fi
echo b


echo $recurisve_cmd

rawOutput=`pwd`/raw
> $rawOutput

"$recursive_cmd" "$1" "$3" "$rawOutput"

fileOut=$HOME/fileout.txt
> $fileOut

sort "$rawOutput" | head -n $2 > $fileOut 
