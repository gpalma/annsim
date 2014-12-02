#!/bin/bash

Dirlist=$(ls ../test/gpcrAnnt/)

index=0
for direc in $Dirlist ; do
      array[$index]=$direc
      index=$index+1
done

rm results.txt
for (( i = 0 ; i < ${#array[@]}-1 ; i++ )) do
for (( j = $i+1 ; j < ${#array[@]}  ; j++ )) do
../annsim ../test/gpcr/gpcr_simmat_target ../test/gpcr/gpcr_desc_term ../test/gpcrAnnt/${array[$i]} ../test/gpcrAnnt/${array[$j]} >> results.txt &
done
done


