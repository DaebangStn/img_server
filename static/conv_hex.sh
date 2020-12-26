#!/bin/bash

input=${1}
width=120
height=30

width_=$((width-1))
height_=$((height-1))

rm -r "./"$input
ffmpeg -i $input".gif" -vsync 0 $input"%02d.bmp"

mkdir "conv_"$input
mv *.bmp "./conv_"$input

cd "./conv_"$input

for a in $(ls)
do
	ffmpeg -i $a -vf scale=120:30 $a -y
	b=$(xxd -s 0x36 -p $a | xxd -r -p)
	a=${a:0:-4}
	echo -n $b >> $a"_"

  for i in $(seq 0 $height_)
  do
    for j in $(seq 0 $width_)
    do
      offset=$((4*(i*width+j)))
			s=${b:$offset:3}
			echo -n $s >> $a"_"$j
    done
  done
done

mkdir bmp
mv *.bmp "./bmp"