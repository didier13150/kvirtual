#!/bin/bash
# 
# Convert svg file into png
#
# Without args, convert all svg file in directory 
# 
# Didier Fabert
#
SIZE="22x22"

function convert_svg() {
	local svg=$1
	basename=${svg%.svg}
	echo "convert -background none -resize ${SIZE} $svg $basename.png"
	convert -background none -resize ${SIZE} $svg $basename.png
}

ARG=$@
if [[ "${ARG}" == "" ]]
then
	ARG=$(ls *.svg) 
fi
for svgfile in ${ARG}
do
	convert_svg $svgfile
done
exit 0
