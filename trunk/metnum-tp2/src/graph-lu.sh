#!/bin/bash
# $1...$n son los archivos de test para cada corrida. Como cada corrida precisa
# dos archivos, n debe ser siempre multiplo de 2

OUT="out"
TIME=$(which time)

argc=$#
if [ $argc -eq 0 -o ! \( `expr $argc % 2` -eq 0 \) ]; then
	echo "Los parametros deben ser multiplos de 2 (2 archivos por corrida)"
	exit 1
fi

if [ -z $TIME ]; then
	echo "Debe instalar el comando time (de GNU)"
	exit 2
fi

rm -f $OUT/lu_*
mkdir -p $OUT

# argc es el doble de los archivos por corrida. Y se necesitan dos archivos por
# corrida. Luego, argc/4 es la cantidad de corridas
niter=$(expr $argc / 2)

for i in $(seq 1 $niter); do
	echo "Corriendo ./lu $1 $2"
	mem_and_time=$($TIME -f "%M %e" ./lu $1 $2 2>&1 > /dev/null)
	mem=$(echo $mem_and_time | cut -d ' ' -f1)
	timee=$(echo $mem_and_time | cut -d ' ' -f2)

	matrix_n=$(head -n1 $1 | fromdos -d)
	echo $matrix_n $mem >> $OUT/lu_mem
	echo $matrix_n $timee >> $OUT/lu_time

	shift 2
done
