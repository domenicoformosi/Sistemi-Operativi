#!/bin/bash

if [[ ! $# -eq 2 ]]; then echo "Mi aspetto 2 parametri in ingresso..";exit;fi   
if [[ ! -d $1 ]];then echo "Mi aspetto una directory esistente..";exit;fi
if [[ $1 != /* ]];then echo "Mi aspetto una directory assoluta..";exit;fi
if [[ $2 -le 0 ]];then echo "Mi aspetto un intero positivo..";exit;fi

if [[ "$0" = /* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/rec.sh"
elif [[ "$0" = */* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/rec.sh"
else
    recursive_cmd=rec.sh
fi
> $HOME/output.txt
"$recursive_cmd" "$1" "$2" "$HOME"
