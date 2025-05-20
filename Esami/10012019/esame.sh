#!/bin/bash

if [[ ! $# -eq 3 ]]; then echo "Mi aspettavo 3 parametri (piange istericamente)"; exit; fi
if [[ ! -d $1 ]]; then echo "Mi aspettavo una directory (piange istericamente)"; exit; fi
if [[ $2 =~ ^[0-9]+$ ]];then "Mi aspettavo una stringa di caratteri (piange istericamente)"; exit; fi
if [[ $3 -le 0 ]]; then echo "Mi aspettavo un intero positivo (piange istericamente)"; exit; fi


if [[ "$0" = /* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/rec.sh"
elif [[ "$0" = */* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/rec.sh"
else
    recursive_cmd=rec.sh
fi

esito=`pwd`/esito.out
# creo esito qua perche rec deve scrivere sempre la, se lo creo in rec poi si cancella ogni volta pwd a sto punto non dovrei passarlo ma non ho voglia di cambiare
"$recursive_cmd" "$1" "$2" "$3" "`pwd`" "$esito"

# path script ricorsivo, path directory,stringa, N e path assoluto dove mi trovo

