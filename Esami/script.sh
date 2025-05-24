find . -type d | while read dir; do
    base=$(basename "$dir")
    parent=$(dirname "$dir")

    if [[ $base =~ ^([0-9]{2})([0-9]{2})([0-9]{4})(.*)$ ]]; then
        day=${BASH_REMATCH[1]}
        month=${BASH_REMATCH[2]}
        year=${BASH_REMATCH[3]}
        rest=${BASH_REMATCH[4]}
        rest=${rest#_}  # rimuove underscore iniziale se presente

        newname="${year}-${month}-${day}"
        [[ -n $rest ]] && newname="${newname}_${rest}"

        newpath="${parent}/${newname}"
        if [[ "$dir" != "$newpath" ]]; then
            mv "$dir" "$newpath"
        fi
    fi
done

