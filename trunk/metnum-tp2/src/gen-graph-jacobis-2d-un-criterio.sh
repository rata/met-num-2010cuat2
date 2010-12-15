#!/bin/bash

for i in `seq 1 20`; do

	./jacobis ../tests/oficiales/dc9-page.tp2 ../tests/oficiales/dc9-pagelinks.tp2 1e-$i
done
