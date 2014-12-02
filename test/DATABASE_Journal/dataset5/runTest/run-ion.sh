#!/bin/bash

Dirlist=$(ls ../test/ionAnnt/)

index=0
for direc in $Dirlist ; do
      array[$index]=$direc
      index=$index+1
done

rm results.txt
for (( i = 0 ; i < ${#array[@]}-1 ; i++ )) do
for (( j = $i+1 ; j < ${#array[@]}  ; j++ )) do
#echo ${array[$i]} ${array[$j]}
../annsim ../test/ion/ic_simmat_target ../test/ion/ic_desc_term ../test/ionAnnt/${array[$i]} ../test/ionAnnt/${array[$j]} >> results.txt &
done
done


