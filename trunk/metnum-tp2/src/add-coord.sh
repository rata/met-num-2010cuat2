#!/bin/bash

i=0

for line in $(cat out/lu-per-line-dc9); do
	i=$((i+1))
	for j in `seq 1 33`; do
	       echo $j $i $line >> out/lu-one-iter-dc9
	done

done
