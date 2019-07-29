#!/bin/bash
scenes="/mnt/seenas2/data/intelScenes/Scenes/"
abstract="/mnt/seenas2/data/intelScenes/Scenes/Abstract"
nature="/mnt/seenas2/data/intelScenes/Scenes/Nature"
city="/mnt/seenas2/data/intelScenes/Scenes/City"

for i in $(ls $abstract);do
    if [[ $i == *.obj ]];then
        absuseables="$absuseables "$abstract/$i""
    fi
done

for i in $(ls $nature);do
    if [[ $i == *.obj ]];then
        natuseables="$natuseables "$nature/$i""
    fi
done

for i in $(ls $city);do
    if [[ $i == *.obj ]];then
        cituseables="$cituseables "$city/$i""
    fi
done

useables="$absuseables $natuseables $cituseables"
exec /opt/ospray-1.8.5/build/ospExampleViewer $useables pt
