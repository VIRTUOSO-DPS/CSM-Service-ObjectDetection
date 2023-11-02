# CSM Service Object Detection Development Guide

## Compiling

The following instructions describe the required steps to build the project on Linux and Nvida Jetson (Nano).

### Linux

To compile, make sure that OpenCV is available, developed and tested against versions:

- 4.8.1

Additionally, the CUDA Toolkit needs to be available, developed and tested against versions:

- 12.2

Crow requires ASIO to be installed, developed and tested against versions:

- 1.28.0

On Arch Linux, these dependencies are installed as follows.

```bash
yay -S cuda opencv-cuda asio
```

Assuming the availability of _yay_.

Next, the project can be compiled using CMake. First, create a _build_ directory:

```bash
mkdir build && cd build
```

Next, use CMake for generation and building:

```bash
cmake ../
```

It may be necessary to provide more information to CMake for locating CUDA, for example:

```bash
cmake -DCUDAToolkit_ROOT=/opt/cuda/bin/ -DCMAKE_CUDA_COMPILER=/opt/cuda/bin/nvcc ../
```

Next, the project can be built:

```bash
cmake --build .
```

### Nvidia Jetson

Please refer to the instructions for building the Nvidia Jetson Nano Docker image below.

## Building Docker images

Docker files for building Docker images are provided in the _docker_ folder.

### Nvidia Jetson Nano

The _docker_ folder contains a Dockerfile for building a Docker image for the Nvidia Jetson Nano.
The image can be built as follows.

```bash
docker build -f Dockerfile -t csm-service-objectdetection:latest .
```

The _CACHEBUST_ argument can be provided to avoid caching the buid stage.

```bash
--build-arg CACHEBUST=$(date +%s)
```

When building the Docker image on a Jetson Nano, make sure to use a sufficiently large SD card (e.g. 128GB).
