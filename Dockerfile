FROM ubuntu:bionic AS base

RUN apt-get update && \
    apt-get install -y \
	build-essential \
	cmake \
	libopenmpi-dev \
	libopenimageio-dev \
	pkg-config \
	make \
	cmake \
	build-essential \
	libz-dev \
	libtbb-dev \
	libglu1-mesa-dev \
	freeglut3-dev \
	libnetcdf-c++4-dev \
	xorg-dev \
        x11-apps \
	xauth \
	x11-xserver-utils \
	vim \
	libjpeg-dev \
	imagemagick \
	python3.7 \
        python3-pip \
    && \
    rm -rf /var/lib/apt/lists/*

RUN python3.7 -m pip install \
    Pillow \
    pywavefront

WORKDIR /opt
ARG ispc_version=1.10.0
ADD ispc-v${ispc_version}-linux.tar.gz /opt/

WORKDIR /opt
ARG embree_version=3.5.2
ADD embree-${embree_version}.x86_64.linux.tar.gz /opt/

WORKDIR /opt
ARG oidn_version=0.8.2
ADD oidn-${oidn_version}.x86_64.linux.tar.gz /opt/

WORKDIR /opt
ARG ospray_version=1.8.5
ADD ospray-${ospray_version}.tar.gz /opt/

WORKDIR /opt/ospray-${ospray_version}/build
COPY ospExampleViewer.cpp /opt/ospray-${ospray_version}/apps/exampleViewer/ospExampleViewer.cpp
RUN cmake .. \
        -DOSPRAY_APPS_ENABLE_DENOISER=1 \
	-DOpenImageDenoise_DIR=/opt/oidn-${oidn_version}.x86_64.linux/lib/cmake/OpenImageDenoise/

RUN make -j$(nproc)
