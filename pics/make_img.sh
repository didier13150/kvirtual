#!/bin/bash

for svg in $(ls *.svg) 
do
	basename=${svg%.svg}
	echo "convert -background none -resize 32x32 $svg $basename.png"
	convert -background none -resize 32x32 $svg $basename.png
done

