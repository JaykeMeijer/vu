#!/bin/bash

# dit is gwn wat random shit
DATA=( cgl cgt fgl fgt lfl lft )
THREADS=1
WORKLOAD=( 0 10 20 30 40 50 )
NUMEL=( 10000 20000 30000 40000 50000 60000 70000 80000 90000 100000)

echo "<data_structure> <nrThreads> <nrItems> <workTime> <runTime>" > file.txt

# super mega loop in loop
# dit is sowieso veels te veel 
for ((i=1; i <= $THREADS; i++ ))
do
    for elem in "${NUMEL[@]}"
    do
        for work in "${WORKLOAD[@]}"
        do
            for data in "${DATA[@]}"
            do
                time=`sh bin/test_data_structures $data $i $elem $work | grep time | egrep -o [0-9]+`
                echo $data $i $elem $work $time >> file.txt
            done
        done
    done
done
