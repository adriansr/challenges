#!/bin/bash

PROGRAM="$1"
INPUT="$2"

total=0
for i in $(seq 1 10)
do
    start=$(gdate +%s%3N)
    $PROGRAM < $INPUT > /dev/null 2>&1
    end=$(gdate +%s%3N)
    total=$(expr $total + $end - $start)
done
echo "Time: " $(expr $total / 10) 
