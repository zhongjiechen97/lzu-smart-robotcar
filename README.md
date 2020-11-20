# lzu-smart-robotcar

This is a smart unmanned robotcar designed based on ROS on software and Jeston nano and Arduino on hardware that can navigate autonomously.

## Tips

In order to use the code in this project, you need to purchase **NVIDIA Jetson nano** and **Arduino development boards** and a **Lidar** by yourself, and assemble a physical vehicle. We will provide a list of the hardware we use in subsequent updates.

## Modules

This project contains three modules.

- **/Arduino** contains the **Arduino code** which **control the robotcar directly.**

- **/ROS** contains the **ROS code** which can **Object Detection(based on YOLO v3)**, **Automatic navigation(based on move_base)**,

- **/Web** contains the **html/css/js code** which **display the sensor data** collected by the robotcar.

If you don’t need to build a Website on the backend server to display data like us, you don’t need to use the code in **/Web**.

## Functions

- Object Detection
\
![](https://github.com/zhongjiechen97/lzu-smart-robotcar/blob/main/Images/object%20detection.png)

- Navigation using Lidar sensor data
\
![](https://github.com/zhongjiechen97/lzu-smart-robotcar/blob/main/Images/navigation.png)
