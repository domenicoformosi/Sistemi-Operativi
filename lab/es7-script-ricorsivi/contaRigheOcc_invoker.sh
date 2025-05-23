#!/bin/bash
if [[ ! -d $1 ]] ;then echo $1 non è una directory!; exit; fi
if [[ $1 != /* ]] then echo $1 non è un nome assoluto! ; exit; fi
if [[ -z $2 ]] then echo mi hai passato stringa nulla broo! ; exit; fi

if [[ "$0" = /* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/contaRigheOcc_recursion.sh"
elif [[ "$0" = */* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/contaRigheOcc_recursion.sh"
else
    recursive_cmd=contaRigheOcc_recursion.sh
fi

"$recursive_cmd" "$1" "$2"