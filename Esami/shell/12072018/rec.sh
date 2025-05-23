cd $1



for i in `ls`
do
    if [[ -f $i ]]
    then
        echo " "`pwd`/$i" "$2" "
        cp "`pwd`/$i" "$2"
    else
        echo "trovato dir $i metto nome in file" 
        `echo $i >> $3`
        "$0" "$i" "$2" "$3"
    fi
done