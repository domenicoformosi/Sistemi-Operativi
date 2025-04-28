#!/bin/bash
cd $HOME 
# aggiungere controllo su basaname
echo $1,$2
if [[ "$1" = */* ]] ;then echo $1 non è basename; exit; fi
if [[ "$2" = */* ]] ; then echo $2 non è basename; exit; fi
if [ $# -ne 2 ] ; then echo Sintassi! ; exit; fi
if [[ ! -f "$1" || ! -f "$2" ]] ; then echo File inesistenti! ; exit; fi

read user

userFile1=`ls -l "$1" | awk '{ print $3 }'`
userFile2=`ls -l "$2" | awk '{ print $3 }'`

echo proprietario di "$1" è $userFile1
echo proprietario di "$2" è $userFile2

if [[ "$user" !=  $userFile1 || $user != $userFile2 ]];
    then echo Proprietari non coincidono!;
    exit;
fi

dimFile1=`stat -c %s "$1"`
dimFile2=`stat -c %s "$1"`
absPath1=$PWD\/"$1"
absPath2=$PWD\/"$2"


if [ $dimFile1 -lt $dimFile2 ]; 
    then echo Il file $absPath2 ha dimensione paggiore, pari a `$dimFile2/1000` Kb;
fi
if [ $dimFile1 -ge $dimFile2 ]
    then  echo Il file $absPath1 ha dimensione maggiore (uguale), pari a `$dimFile1/1000` Kb;
fi
