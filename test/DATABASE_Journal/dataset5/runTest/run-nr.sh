#!/bin/bash

Dirlist=$(ls ../test/nrAnnt/)

index=0
for direc in $Dirlist ; do
      array[$index]=$direc
      index=$index+1
done

rm results.txt
for (( i = 0 ; i < ${#array[@]}-1 ; i++ )) do
for (( j = $i+1 ; j < ${#array[@]}  ; j++ )) do
../annsim ../test/nr/nr_simmat_target ../test/nr/nr_desc_term ../test/nrAnnt/${array[$i]} ../test/nrAnnt/${array[$j]} >> results.txt &
done
done


