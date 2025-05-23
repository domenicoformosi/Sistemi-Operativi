# Script ricorsivi in shell

## Problema E Schema Risolutivo
Supponendo di voler navigare in maniera ricorsiva il sotto albero individuato da `dir`, nelle slide dell'esercitazione 7 si nota come un'implementazione naive può portare diversi problemi (se non viene passata la directory assoluta poi quando cambiamo directory abbiamo problemi dato che non sappiamo come richiamare lo script di ricorsione), viene presentata quindi direttamente la soluzione generale, che utilizza due script

Approfondimennto dalle slide [qua](./es7-bash-AK.pdf)
- **Ivoker** Si occupa di controllare gli argomenti passati in input e invoca il file comandi ricorsivo
- **Recursion** Esegue le operazioni richieste e richiama lo script ricorsivo (se stesso)

## Invoker
Guardiamo la soluzione presentata e commentiamola per capire cosa succede
``` sh
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
```

Possiamo osservare la prima parte di controllo sull'input relativamente arbitraria su cui non mi soffermerò, più cruciale è invece la parte di controllo su `$0`
``` sh
if [[ "$0" = /* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="$dir_name/contaRigheOcc_recursion.sh"
```
questo pezzo controlla che `$0`sia un path assoluto ([Differenza tra path assoluto e relativo](#section-3) ), se è un path assoluto allora tramite `expr` esegue il comando `dirname`, che non fa altro che rimuovere tutto ciò che c'è dopo l'ulitmo slash
```
echo $PWD ==> /home/Sistemi-Operativi

echo `dirname "$PWD"` ==> /home
```
Successivamente a `recursive_cmd` viene assegnato il valore della directory corrente + nome del file ricorsivo 
> Essenzialmente abbiamo ricostruito path (assoluto) del file ricorsivo

``` sh
elif [[ "$0" = */* ]] ; then
    dir_name=`dirname "$0"`
    recursive_cmd="`pwd`/$dir_name/contaRigheOcc_recursion.sh"
```
Qua la condizione è invece verificata se il path è di tipo relativo (non inzia con / ma comunque lo contiene), in questo caso andiamo a generare sempre il path assoluto del recursive_cmd andando a utilizzare `dirname` per ottenere path senza il nome dell'invoker, solo che tramite `pwd`, che [ricordando](#pwd), ci permette di generare anche qui il path assoluto
``` sh
else
    recursive_cmd=contaRigheOcc_recursion.sh
fi
```
Ultima condizione, `$0` non è un path assoluto ne relativo, allora quest'ultimo sara cercato in `$PATH` ([cos'è $PATH?]($PATH))
``` sh
"$recursive_cmd" "$1" "$2"
```
Infine non facciamo altro che richiamare lo script ricorsivo tramite il path appena generato e passando i parametri (che avremmo dovuto controllare)
> 📝 **Nota Bene**:  
> Abbiamo assunnto che invoker e recursive fossero nella stessa directory
## Differenza tra path assoluto e relativo
- **Path assoluto** :È il percorso completo di un file o directory a partire dalla root del file system, indicata con lo slash /.
- **Path relativo** : È il percorso di un file o directory relativo alla posizione corrente (la "working directory").
### pwd 
```
- man pwd
    NAME
        pwd - print name of current/working directory

    SYNOPSIS
        pwd [OPTION]...

    DESCRIPTION
        Print the full filename of the current working directory.
```
### $PATH
La variabile d'ambiente PATH contiene un elenco di directory separate da due punti : dove il sistema cerca gli eseguibili quando digiti un comando nel terminale