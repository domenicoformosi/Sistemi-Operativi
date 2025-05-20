#!/bin/bash

cd "$1"


for i in `ls`
do
    if [[ -d "$i" ]]
    then
        numFile=`ls | grep "$2" | wc -l`
        echo "Dir $i ha $numFile che iniziano con $2"
        if [[ $numFile -gt $3 ]]
        then
            `echo "$PWD $numFile" >> $5`
        fi

        "$0" "$i" "$2" "$3" "$4" "$5"

    fi


done