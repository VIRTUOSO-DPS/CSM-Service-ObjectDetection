# CSM Object Detection

This project provides an object detection service using YOLOv5. The service is specifically designed for Nvidia Jetson Nano,
and used as a local Edge service in CSM experiments. The [BUILD.md](BUILD.md) document contains more information about building
this project.

## Running

After installation, a service named _csm-service-objectdetection.service_ is installed, it can be enabled as follows.

```bash
systemctl daemon-reload
systemctl enable csm-service-objectdetection.service
systemctl start csm-service-objectdetection.service
```
