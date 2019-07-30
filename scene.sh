#!/bin/bash
base=/mnt/seenas2/data/intelScenes/Scenes

[ -f scene.env.sh ] && . scene.env.sh

abstract=${base:?}/Abstract
nature=${base:?}/Nature
city=${base:?}/City
chromeroom=${base:?}/Chrome_Room
spheres=${base:?}/Spheres

for i in $abstract/*.obj; do
    absuseables="$absuseables "$i""
done

for i in $nature/*.obj; do
    natuseables="$natuseables "$i""
done

for i in $city/*.obj; do
    cituseables="$cituseables "$i""
done

for i in $chromeroom/*.obj; do
    chromeusables="$chromeusables "$i""
done

for i in $spheres/*.obj; do
    sphereusables="$sphereusables "$i""
done

useables="$absuseables $natuseables $cituseables $chromeusables $sphereusables"
lights=""
exec /opt/ospray-1.8.5/build/ospExampleViewer $lights $useables -r pt
