#!/bin/bash
strToFind="$2"
> "$HOME/report.log"

if [[ -f "$1" ]] ; then 
    #devo contare numero di righe contententi almeno un occorrenza di strToFind
    numOcc=`grep -c $strToFind $1`
    echo $numOcc
    if [ `expr $numOcc % 2` -eq 0 ] ; then
        absName=$PWD\/"$1"
        echo Il file $absName contiene un numero pari \( $numOcc \) di righe rilevanti
    else
        echo "Il file $absName contiene un numero dispari ( $numOcc ) di righe rilevanti \n" >> "$HOME/report.log"
    fi
elif  [[ -d "$1" ]] ; then 
    cd "$1"
    for f in *; do
        "$0" "$f" "$strToFind"
    done
fi