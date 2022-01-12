#!/bin/sh

cd src
make
./ray out.ppm --cores=8
mv out.ppm ..
