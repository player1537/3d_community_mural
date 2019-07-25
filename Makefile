all: out.png

.mk.docker: server.c
	./go.sh build
	touch $@

out.bin: .mk.docker
	./go.sh run /opt/app/server

out.png: out.bin
	./go.sh run python3.7 -u foo.py
