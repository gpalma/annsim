#!/bin/bash

Dirlist=$(ls ../test/drugs_t_annts/)

index=0
for direc in $Dirlist ; do
      array[$index]=$direc
      index=$index+1
done

rm results.txt
for (( i = 0 ; i < ${#array[@]}-1 ; i++ )) do
for (( j = $i+1 ; j < ${#array[@]}  ; j++ )) do
echo ${array[$i]} ${array[$j]}
../annsim ../test/sim_matrix_drug_target/ligandJaccardDrugs-distTargets.txt ../test/drugs_targets/drug-target-Descp.txt ../test/drugs_t_annts/${array[$i]} ../test/drugs_t_annts/${array[$j]} >> results.txt &
done
done


