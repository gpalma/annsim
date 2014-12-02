#!/bin/bash

Dirlist=$(ls ../test/enzymeAnnt/)

index=0
for direc in $Dirlist ; do
      array[$index]=$direc
      index=$index+1
done

rm results.txt
for (( i = 0 ; i < ${#array[@]}-1 ; i++ )) do
for (( j = $i+1 ; j < ${#array[@]}  ; j++ )) do
echo ${array[$i]} ${array[$j]}
../annsim ../test/enzyme/e_simmat_target ../test/enzyme/e_desc_term ../test/enzymeAnnt/${array[$i]} ../test/enzymeAnnt/${array[$j]} >> results.txt &
done
done


