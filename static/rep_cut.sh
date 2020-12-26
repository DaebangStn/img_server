#!/bin/bash

# ./rep_cut.sh ${filename} ${width} ${height}
# cutting 3bytes for every 4bytes

file=${1}
width=${2}
height=${3}

width=$((width-1))
height=$((height-1))

for i in $(seq 0 $height)
do
	for j in $(seq 0 $width)
	do
		offset=$((4*(i*height+j)))
		xxd -s $offset -l 3 -p $file | xxd -r -p >> $file"cut"$j
	done
done
