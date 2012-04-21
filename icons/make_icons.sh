#!/bin/bash

app=$(ls hisc-app-*.svgz | awk -F '.' '{print $1}' | awk -F '-' '{print $3}')
cp hisc-app-$app.svgz hisc-app-$app.svg.gz
gunzip hisc-app-$app.svg.gz

[ -f hisc-app-$app.svg ] || exit
for iconsize in 16 22 32 48 64 128 
do
	echo "convert -background none -resize ${iconsize}x${iconsize} hisc-app-$app.svg hi${iconsize}-app-$app.png"
	convert -background none -resize ${iconsize}x${iconsize} hisc-app-$app.svg hi${iconsize}-app-$app.png
done

rm -f hisc-app-$app.svg
