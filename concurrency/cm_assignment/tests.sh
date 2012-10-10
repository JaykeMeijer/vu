#!/bin/bash

# dit is gwn wat random shit
DATA=( cgl cgt fgl fgt lfl lft )
THREADS=12
WORKLOAD=( 0 10 20 30 40 50 60 70 80)
NUMEL=( 10000 20000 30000 40000 50000 60000 70000 80000 90000 100000)

echo "<data_structure> <nrThreads> <nrItems> <workTime> <runTime>" > file.txt

for ((i=1; i <= $THREADS; i++))
do
    for data in "${DATA[@]}"
    do
        time=`sh bin/test_data_structures $data $i 10000 30 | grep time | egrep -o [0-9]+`
        echo $data $i 10000 10 $time >> file.txt
    done
done

for elem in "${NUMEL[@]}"
do
    for data in "${DATA[@]}"
    do
        time=`sh bin/test_data_structures $data 4 $elem 30 | grep time | egrep -o [0-9]+`
        echo $data 4 $elem 30 $time >> file.txt
    done
done

for work in "${WORKLOAD[@]}"
do
    for data in "${DATA[@]}"
    do
        time=`sh bin/test_data_structures $data 4 $elem 30 | grep time | egrep -o [0-9]+`
        echo $data 4 10000 $work $time >> file.txt
    done
done
