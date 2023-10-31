# CSM Service (Edge) Object Detection

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
