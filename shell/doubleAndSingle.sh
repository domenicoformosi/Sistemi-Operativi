#!/bin/bash

# Crea alcuni file di prova
touch result1 res respo response

# Imposta una variabile
a="result1"

echo "----- USANDO [[ ... ]] -----"
if [[ $a == res* ]]; then
  echo "[[ $a == res* ]] => MATCH (funziona!)"
else
  echo "[[ $a == res* ]] => NO match"
fi

echo
echo "----- USANDO [ ... ] -----"
if [ $a = res* ]; then
  echo "[ $a = res* ] => MATCH (ma attenzione)"
else
  echo "[ $a = res* ] => NO match"
fi

echo
echo "----- USANDO [ ... ] con wildcard multipli -----"
# Simula caso con più file che matchano res*
echo "Aggiungendo un altro file res_extra..."
touch res_extra

# Questo genererà errore se ci sono più match!
echo "Eseguo: [ \$a = res* ]"
[ $a = res* ]
echo "=> Esito comando precedente: $?"

# Cleanup finale (opzionale)
rm -f result1 res respo response res_extra