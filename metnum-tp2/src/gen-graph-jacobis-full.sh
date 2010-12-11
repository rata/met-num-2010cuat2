

for i in $@; do
	ARGS="$ARGS $i-page.tp2 $i-pagelinks.tp2 "
done

echo ./graph-jacobis.sh $ARGS > test.sh
