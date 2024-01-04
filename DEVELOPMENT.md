# CSM Service Object Detection Development Guide

## Compiling

The following instructions describe the required steps to build and install the service on Nvida Jetson (Nano).

### Nvidia Jetson Nano

OpenCV compiled with CUDA and cuDNN support is required for fast processing. To ease the process of building OpenCV,
a script _scripts/build-opencv.sh_ is provided. Compilation time is significant, it may pay off to, instead of removing
the build root generate a DEB package after compilation.

The ASIO development libraries need to be installed. Unfortunately, no recent enough version of ASIO is provided in the
Ubuntu 18.04 (which the L4T image is based on) package repository, and ASIO needs to be built from the sources. This is
fairly straightforward, however.

Acquire the ASIO source code from the website: https://think-async.com/Asio/, after acquiring the sources proceed as
follows.

```bash
./configure
make
make install
```

Next, this project can be built as follows.

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .
```

It may be required to export the following variables:

```bash
export CUDA_HOME="/usr/local/cuda"
export PATH="/usr/local/cuda/bin:${PATH}"
export LD_LIBRARY_PATH="/usr/local/cuda/lib64:${LD_LIBRARY_PATH}"
```

A service named _csm-service-objectdetection.service_ is installed, it can be enabled as follows.

```bash
systemctl daemon-reload
systemctl enable csm-service-objectdetection.service
systemctl start csm-service-objectdetection.service
```
