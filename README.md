# CSM Service (Edge) Object Detection

This project is part of a larger distributed application designed for the cloud-edge-IoT continuum. Our primary
goal is to harness the power of YOLOv5, a state-of-the-art object detection model, on the NVIDIA Jetson Nano,
a compact yet powerful edge computing device. By using the GPU on the Jetson Nano, we can perform efficient and
real-time object detection, making it perfect for a wide range of applications.

## About YOLOv5

YOLO (You Only Look Once) is a popular family of real-time object detection models, and YOLOv5 is the latest
iteration. It offers impressive accuracy and speed, making it a perfect choice for edge computing scenarios.
The model is designed to work with a variety of objects and can be customized for specific use cases, such as
detecting people, vehicles, animals, or any other object of interest.

## Why Edge Computing with YOLOv5?

Edge computing brings data processing closer to the source, reducing latency and minimizing the need for constant
cloud connectivity. By running YOLOv5 on the NVIDIA Jetson Nano, we can achieve real-time, on-device object
detection, which is crucial for applications like:

- Surveillance systems
- Smart security cameras
- Autonomous drones and vehicles
- Industrial automation
- Agricultural monitoring
- Healthcare and telemedicine

## Features

- Live object detection using YOLOv5 on the NVIDIA Jetson Nano.
- Easy-to-use web service for initiating inference requests.
- Utilizes GPU acceleration for faster and more efficient processing.
- Designed for seamless integration into cloud-edge-IoT applications.
