#!/bin/bash

if [[ -z $1 ]]; then echo "Sintassi, stringa nulla"; exit; fi
if [[ $2 -le 0]]; then echo "Sintassi, valore illegale"; exit; fi

str=$1
M=$2

shift 
shift

for $i in $*;
    do 
        if [[ $i != /* ]] ; then echo "Sintassi, dir non ass"; exit; fi
        if [[ -d $i ]] ; then echo "Sintassi, dir non esistente"; exit; fi
    done

./topclear

