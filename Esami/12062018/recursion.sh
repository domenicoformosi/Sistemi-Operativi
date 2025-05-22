#!/bin/bash
cd "$1"
echo "dir attuale $1 == `pwd`"
for i in `ls` 

    do 
        echo "controllo $i"
        if [[ -f $i ]]; then

            echo "$i è un file vedo se ha almeno $3 $2"

            wordCount=`grep "$2" "$i" | wc -w`

            echo $wordCount

            if [[ $wordCount -gt $3 ]]; then
                echo " `pwd`/"$i" $wordCount"
            fi

        elif [[ -d $i ]]; then
            echo "Trovato sotto dir, controllo anche lei "
            "$0" "$i" "$2" "$3"

        else
            echo "Che mi hai passato bro"
        fi
    done

# mi sposto nella sotto directory, per ogni elemento di questa
# se è un file faccio i miei controlli ed eventuale stampa
# se è una directory richiamo il programma riavviando recursion