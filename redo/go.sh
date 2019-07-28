#!/usr/bin/env bash

tag=mural_redo:$USER
name=mural_redo_$USER
target=
data=
registry=
xauth=
entrypoint=
ipc=
net=
user=1
cwd=1
interactive=1
script=
port=8801
network=mural_redo_$USER

if [ -f env.sh ]; then
	. env.sh
fi

build() {
	docker build \
		${target:+--target $target} \
		-t $tag .
}

run() {
	if [ -n "$xauth" ]; then
		rm -f $xauth
		xauth nlist $DISPLAY | sed -e 's/^..../ffff/' | xauth -f $xauth nmerge -
	fi
	docker run --rm \
		${interactive:+-it} \
		${script:+-a stdin -a stdout -a stderr --sig-proxy=true} \
		${ipc:+--ipc=$ipc} \
		${net:+--net=$net} \
		${user:+-u $(id -u):$(id -g)} \
		${cwd:+-v $PWD:$PWD -w $PWD} \
		${port:+-p $port:$port} \
		${data:+-v $data:$data} \
		${xauth:+-e DISPLAY -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro -v /etc/shadow:/etc/shadow:ro -v /etc/sudoers.d:/etc/sudoers.d:ro -v $xauth:$xauth -e XAUTHORITY=$xauth} \
		${entrypoint:+--entrypoint $entrypoint} \
		$tag "$@"
}

inspect() {
	entrypoint='/bin/bash -i' run "$@"
}

script() {
	interactive= script=1 run "$@"
}

network() {
	docker network create --driver overlay ${network:?}
}

push() {
	docker tag $tag $registry/$tag
	docker push $registry/$tag
}

create() {
	docker service create \
		--name $name \
		--mount type=bind,src=$PWD,dst=$PWD \
		${data:+--mount type=bind,src=$data,dst=$data} \
		${port:+--publish $port:$port} \
		${network:+--network $network} \
		$registry/$tag "$@"
}

destroy() {
	docker service rm $name
}

logs() {
	docker service logs $name "$@"
}

python3.7() { run python3.7 -u "$@"; }
python3() { python3.7 "$@"; }
python() { python3 "$@"; }
server() {
	python server.py \
		${port:+--port=$port} \
		--materials materials.txt \
		--objs objs.txt \
		/opt/app/server \
		"$@"
}

"$@"
