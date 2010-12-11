#!/bin/bash
# $1...$n son los archivos de test para cada corrida. Como cada corrida precisa
# dos archivos, n debe ser siempre multiplo de 4 (2 archivos por corrida, 2
# programas a correr)

OUT="out"

declare -A algo
algo["lu"]=1
algo["jacobis"]=1
algolist=${!algo[*]}

calc_mem() {
	pid=$1
	max_mem=0

	# Mientras el proceso siga ejecutandose
	while [ -e /proc/$pid/ ]; do
		mem=$(ps h -o rss --pid $pid)
		
		# race: puede haber muerto entre el while y que nos fijamos la
		# memoria. Si eso paso, entonces mem es vacio y no hay nada para
		# hacer
		if [ -z $mem ]; then
			continue
		fi

		if [ $mem -gt $max_mem ]; then
			max_mem=$mem
		fi

		sleep 1
	done
}


argc=$#
if [ $argc -eq 0 -o ! \( `expr $argc % 4` -eq 0 \) ]; then
	echo "Los parametros deben ser multiplos de 4 (2 archivos por corrida, 2 \
		programas)"
	exit 1
fi

mkdir -p $OUT

niter=$(expr $argc / 4)

for algo in $algolist; do
	for i in $(seq 1 $niter); do
		echo "Corriendo ./$algo $1 $2"
		./$algo $1 $2 > /dev/null 2>&1 &
		pid=$!
		matrix_n=$(head -n1 $1)

		# calc_mem define la variable max_mem
		calc_mem $pid
	
		echo $matrix_n $max_mem >> $OUT/${algo}_mem
	
		shift 2
	done
done
