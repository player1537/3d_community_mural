FROM ubuntu:bionic AS base

ARG ospray_version=1.8.5

RUN apt-get update && apt-get install -y \
        python3.7 \
        python3-pip \
    && rm -rf /var/lib/apt/lists/*

COPY ospray-${ospray_version}.x86_64.linux.tar.gz /tmp/
RUN tar xvf /tmp/ospray-${ospray_version}.x86_64.linux.tar.gz --strip-components=1 -C /usr/

RUN python3.7 -m pip install \
        Pillow \
        pywavefront

WORKDIR /opt/app

COPY server.c ./
RUN make server \
        CFLAGS='-pedantic -Wall -Werror' \
        LDLIBS='-lospray'

ENTRYPOINT []
CMD []


FROM base AS dev

ENTRYPOINT []
CMD []


FROM base AS dist

WORKDIR /opt/app
COPY server.py ./
COPY static ./static
COPY gen ./gen
COPY materials.txt objs.txt ./

ENTRYPOINT ["python3.7", "-u", "server.py", "--port=8801", "/opt/app/server", "--materials", "materials.txt", "--objs", "objs.txt"]
CMD []
