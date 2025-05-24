#!/bin/bash

str="$1"
M=$2

if [[ $# -lt 3 ]]; then echo "Secondo i miei calcoli il valoe minimo di parametri è 3.."; exit; fi
if [[ ! $str ]]; then echo "Sintassi! stringa nulla"; exit; fi
if [[ $M -le 0 ]]; then echo "Sintassi! valore illegale"; exit; fi

shift 2
# shifto di due posizioni per levarmi dai coglioni valori e fare for su dir

for i in $@ 
    do
        if [[ ! -d "$i" ]]; then echo "$i non è una directory!"; exit; fi
        if [[ "$i" != /* ]]; then echo "$i è una directory ma non assoluta"; exit; fi
    done

top -b > processi.out &

if [[ "$0" = /* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/recursion.sh"
elif [[ "$0" = */* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/recursion.sh"
else
    recursive_cmd=recursion.sh
fi



    
for i in $@
    do
        "$recursive_cmd" "$i" "$str" "$M"
    done


kill `ps | grep top | cut -d' ' -f3`

