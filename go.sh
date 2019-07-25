#!/usr/bin/env bash

tag=mural:$USER
name=mural_service
target=dev
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
port=

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

push() {
	docker tag $tag $registry/$tag
	docker push $registry/$tag
}

create() {
	docker service create \
		--name $name \
		--mount type=bind,src=$PWD,dst=$PWD \
		${data:+--mount type=bind,src=$data,dst=$data} \
		$registry/$tag "$@"
}

destroy() {
	docker service rm $name
}

logs() {
	docker service logs $name "$@"
}

"$@"
