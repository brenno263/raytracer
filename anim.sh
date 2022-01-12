#!/bin/bash

cd src
make

for i in $(seq 0 $(expr $1 - 1))
do
	./ray anim$i.ppm --cores=8 --animate $1 $i
	mv anim$i.ppm ..
done
