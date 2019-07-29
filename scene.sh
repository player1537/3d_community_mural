#!/bin/bash
base=

[ -f scene.env.sh ] && . scene.env.sh

abstract=${base:?}/Abstract
nature=${base:?}/Nature
city=${base:?}/City

for i in $abstract/*.obj; do
    absuseables="$absuseables "$i""
done

for i in $nature/*.obj; do
    natuseables="$natuseables "$i""
done

for i in $city/*.obj; do
    cituseables="$cituseables "$i""
done

useables="$absuseables $natuseables $cituseables"
exec /opt/ospray-1.8.5/build/ospExampleViewer $useables -r pt
