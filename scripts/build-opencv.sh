#!/bin/bash

# OpenCV dependencies
apt-get update \
    && apt purge -y *libopencv* cmake \
    && apt-get install -y --no-install-recommends \
    build-essential git unzip pkg-config zlib1g-dev wget \
    python3-dev python3-numpy \
    python-dev python-numpy \
    gstreamer1.0-tools libgstreamer-plugins-base1.0-dev \
    libgstreamer-plugins-good1.0-dev \
    libtbb2 libgtk-3-dev libxine2-dev \
    libjpeg-dev libjpeg8-dev libjpeg-turbo8-dev \
    libpng-dev libtiff-dev libglew-dev \
    libavcodec-dev libavformat-dev libswscale-dev \
    libgtk2.0-dev libgtk-3-dev libcanberra-gtk* \
    python3-pip \
    libxvidcore-dev libx264-dev \
    libtbb-dev libdc1394-22-dev libxine2-dev \
    libv4l-dev v4l-utils qv4l2 \
    libtesseract-dev libpostproc-dev \
    libavresample-dev libvorbis-dev \
    libfaac-dev libmp3lame-dev libtheora-dev \
    libopencore-amrnb-dev libopencore-amrwb-dev \
    libopenblas-dev libatlas-base-dev libblas-dev \
    liblapack-dev liblapacke-dev libeigen3-dev gfortran \
    libhdf5-dev libprotobuf-dev protobuf-compiler \
    libgoogle-glog-dev libgflags-dev \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

# CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && apt-get update \
    && apt-get install -y --no-install-recommends cmake

cd /tmp/

# Clone OpenCV sources
git clone --depth=1 --branch 4.8.0 https://github.com/opencv/opencv.git \
    && git clone --depth=1 --branch 4.8.0 https://github.com/opencv/opencv_contrib.git

cd /tmp/opencv/

# Make build directory
mkdir /tmp/opencv/build && cd /tmp/opencv/build

# Build
cmake -DCMAKE_BUILD_TYPE=Release \
    -DOPENCV_EXTRA_MODULES_PATH=/tmp/opencv_contrib/modules \
    -DEIGEN_INCLUDE_PATH=/usr/include/eigen3 \
    -DWITH_OPENCL=OFF \
    -DCUDA_ARCH_BIN=5.3 \
    -DCUDA_ARCH_PTX="sm_53" \
    -DWITH_CUDA=ON \
    -DWITH_CUDNN=ON \
    -DWITH_CUBLAS=ON \
    -DENABLE_FAST_MATH=ON \
    -DCUDA_FAST_MATH=ON \
    -DOPENCV_DNN_CUDA=ON \
    -DENABLE_NEON=ON \
    -DWITH_QT=OFF \
    -DWITH_OPENMP=ON \
    -DBUILD_TIFF=ON \
    -DWITH_FFMPEG=ON \
    -DWITH_GSTREAMER=ON \
    -DWITH_TBB=ON \
    -DBUILD_TBB=ON \
    -DBUILD_TESTS=OFF \
    -DWITH_EIGEN=ON \
    -DWITH_V4L=ON \
    -DWITH_LIBV4L=ON \
    -DWITH_PROTOBUF=ON \
    -DOPENCV_ENABLE_NONFREE=ON \
    -DINSTALL_C_EXAMPLES=OFF \
    -DINSTALL_PYTHON_EXAMPLES=OFF \
    -DPYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
    -DOPENCV_GENERATE_PKGCONFIG=ON \
    -DBUILD_EXAMPLES=OFF \
    ..

make -j$(($(nproc) - 1))
make install

# Remove sources and build output
rm -rf /tmp/opencv && rm -rf /tmp/opencv_contrib