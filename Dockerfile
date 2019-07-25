FROM ubuntu:bionic AS base

ARG ospray_version=1.8.5

RUN apt-get update && apt-get install -y \
        python3.7 \
        python3-pip \
    && rm -rf /var/lib/apt/lists/*

COPY ospray-${ospray_version}.x86_64.linux.tar.gz /tmp/
RUN tar xvf /tmp/ospray-${ospray_version}.x86_64.linux.tar.gz --strip-components=1 -C /usr/

RUN python3.7 -m pip install \
        numpy

WORKDIR /opt/app

COPY server.c ./
RUN make server \
        LDLIBS='-lospray'

ENTRYPOINT []
CMD []


FROM base AS dev

ENTRYPOINT []
CMD []


FROM base AS dist

ENTRYPOINT []
CMD []
