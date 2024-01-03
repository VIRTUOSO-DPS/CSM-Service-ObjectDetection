# Nvidia dependencies stage
FROM nvcr.io/nvidia/l4t-jetpack:r35.1.0 AS nvidia-dependencies

# Build OpenCV stage
FROM nvidia-dependencies AS build-opencv

# OpenCV
RUN apt-get update \
    && apt purge -y *libopencv* \
    && apt-get install -y --no-install-recommends \
    build-essential git unzip pkg-config zlib1g-dev \
    python3-dev python3-numpy \
    python-dev python-numpy \
    gstreamer1.0-tools libgstreamer-plugins-base1.0-dev \
    libgstreamer-plugins-good1.0-dev \
    libtbb2 libgtk-3-dev libxine2-dev \
    cmake \
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

WORKDIR /tmp

RUN git clone --depth=1 --branch 4.8.0 https://github.com/opencv/opencv.git \
    && git clone --depth=1 --branch 4.8.0 https://github.com/opencv/opencv_contrib.git
WORKDIR /tmp/opencv

RUN mkdir build
WORKDIR /tmp/opencv/build

RUN cmake -DCMAKE_BUILD_TYPE=Release \
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

RUN make -j$(($(nproc) - 1))
RUN make install

RUN rm -rf /tmp/opencv \
    && rm -rf /tmp/opencv_contrib

ENV CUDA_HOME="/usr/local/cuda"
ENV PATH="/usr/local/cuda/bin:${PATH}"
ENV LD_LIBRARY_PATH="/usr/local/cuda/lib64:${LD_LIBRARY_PATH}"

# Build OpenCV stage
FROM build-opencv AS build-dependencies

# Build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    git libasio-dev cmake build-essential \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

# Build stage
FROM build-dependencies AS build

WORKDIR /app

RUN git clone --recursive https://github.com/VIRTUOSO-DPS/CSM-Service-ObjectDetection.git
WORKDIR /app/CSM-Service-ObjectDetection

RUN mkdir build
WORKDIR /app/CSM-Service-ObjectDetection/build

RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cmake --build .

# Run stage
FROM build-opencv AS run

COPY --from=build /tmp/opencv/build/ /tmp/

COPY --from=build /app/CSM-Service-ObjectDetection/build/csm-service-objectdetection /app/service
COPY --from=build /app/CSM-Service-ObjectDetection/resources/ /app/resources/

WORKDIR /app

EXPOSE 8001

ENTRYPOINT [ "/app/service" ]
