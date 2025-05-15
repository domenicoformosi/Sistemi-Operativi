#!/bin/bash
numOccorrenze = $3
strToFind = $2
> "$4/all.out"
if [[ -f $1 ]]; then

wordCount=`grep -o -i "$2" "$1" | wc - l`
if [[ $wordCount -gt $numOccorrenze]]; then
    `cp $1 $4`
cd $4
echo "$PWD $wordCount" > "all.out"
elif [[ -d $1 ]]; then
    cd $1
    for f in *; do 
    "$0" "$f" "$strToFind" "$numOccorenze" "$4"
    done
fi

