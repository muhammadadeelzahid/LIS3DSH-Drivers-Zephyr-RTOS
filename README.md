# LIS3DSH Drivers for Zephyr RTOS 



This repository contains drivers setup for LIS3DSH sensor based on Zephyr's official example application. The main purpose of this
repository is to serve as a reference on how to setup the structure of your files for Out-of-Tree Drivers for a sensor, in our case we demonstrate for LIS3DSH sensor. More information about Zephyr SPI interfacing can be found at [Oxeltech's blog](https://www.oxeltech.de/en/blogs/using-zephyr-os-for-interfacing-an-imu-sensor-with-nrf52-over-spi)
  
  
  
 ## Getting Started



Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html). First step, you should clone the example-application from [Example-Application](https://github.com/zephyrproject-rtos/example-application). Follow the instructions on the example-application page to initialise a workspace. After this  you can follow detailed instructions and guidelines on our blog [link](https://www.oxeltech.de/en/blogs) on how to setup your config and driver files. You can download individual files and folders from this repository and replace them with the default ones inside the example-application.



## Steps

1.	Add a folder called lis3dsh inside of example-application/drivers/sensor/
2.	Clone and add the lis3dsh.c and lis3dsh.h files inside the lis3dsh folder
3.	Clone and add CMakeLists.txt and Kconfig from drivers/sensor/lis3dsh/CMakeLists.txt and drivers/sensor/lis3dsh/Kconfig files to the example-application folder      respectively
4.	Clone and add the st,lis3dsh.yaml file to the dts/bindings/sensor directory in the example-application project
5.	Clone and add the nrf52dk_nrf52832.overlay file to example-application/app/boards directory. 
6.	Add samples/sensor/lis3dsh/prj.conf from this repository to example-application/app/
7.	Clone and replace the main.c source code to the example-application/app/src folder.



### Build & Run

The application can be built and flashed by running:

```shell
west build -b nrf52dk_nrf52832 app
```

```shell
west flash
```
