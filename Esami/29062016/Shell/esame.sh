if [[ ! -d $1 ]]; then echo Sintassi! $1 non è una direcotry; exit; fi
if [[ ! -d $4 ]]; then echo Sintassi! $4 non è una direcotry; exit; fi
if [[ $2 =~ ^[0-9]+$ ]]; then echo Sintassi! $2 contiene dei numeri; exit; fi
if [[ $3 -le 0 ]]; then echo Sintassi! $3 non è un intero positivo !

echo "$0"