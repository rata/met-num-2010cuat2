src=$1 # imagen original
dst=$2 # ascii file
w=$3   # ancho
h=$4   # alto
k=$5   # k (default 3)

DIR=$(dirname $0)

if [ x"$h" = "x" ]; then
	echo "Usage $0 <src> <dst> <width> <height> [k]"
	exit 1
fi

if [ x"$k" = "x" ]; then
	k=3
fi

new_w=$(expr $w '*' 11)
new_h=$(expr $h '*' 19)
new_size="${new_w}x${new_h}"
new_src="$src.gray"

$DIR/to-raw.sh $src $new_src $new_size
echo "Raw img generated, size: $new_size"

if [ ! -e $DIR/../data/autoVal1.dat ]; then
	$DIR/gen-data 11 19 $DIR/../imgs/*.gray
	echo Datos generados.
fi

$DIR/art-attack $new_src $w $h $dst $k

rm $new_src
