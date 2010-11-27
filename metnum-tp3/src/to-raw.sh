
src=$1
dst=$2 # DEBE terminar en .gray
size=$3

convert $src -resize $size\! -density 95 -depth 8 $dst
