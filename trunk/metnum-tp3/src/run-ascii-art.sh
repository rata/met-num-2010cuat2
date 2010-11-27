src=$1 # imagen original
dst=$2 # ascii file
w=$3   # ancho
h=$4   # alto

DIR=$(dirname $0)

if [ x"$h" = "x" ]; then
	echo "Usage $0 <src> <dst> <width> <height>"
	exit 1
fi

new_w=$(expr $w '*' 11)
new_h=$(expr $h '*' 19)
new_size="${new_w}x${new_h}"
new_src="$src.gray"

$DIR/to-raw.sh $src $new_src $new_size

#if [ ! -e $DIR/asdasd ]; then
#	echo generar el archivo usando las imagenes de prueba
#fi

$DIR/ascii-art $new_src $new_w $new_h $dst
