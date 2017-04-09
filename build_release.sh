#!/bin/sh

[ -d ./release ] && rm -rf ./release
mkdir ./release

cd ./release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd -
