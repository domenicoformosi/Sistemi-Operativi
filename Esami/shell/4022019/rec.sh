#! /bin/bash

cd "$1"

numFile=0;
for i in `ls`
do 
    if [[ -f "$i" ]]
    then 
        owner=`stat -c %u "$i"`
        echo "controllo $i"
        if [[ $owner = $2 ]]
        then
            numFile=`expr $numFile + 1`
            echo "Numero di file di $2 in $1: $numFile"
        fi
        
    elif [[ -d "$i" ]]
    then 
        "$0" "$i" "$2" "$3"
    fi

done
if [[ $numFile -gt 0 ]]
then
    `echo $PWD $numFile >> $3/output.txt`
fi