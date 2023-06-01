# Sensor Drivers for LIS3DSH using Zephyr RTOS 



This repository contains drivers setup for LIS3DSH sensor based on Zephyr's official example application. The main purpose of this
repository is to serve as a reference on how to setup the structure of your files for Out-of-Tree Drivers for a sensor, in our case we demonstrate for LIS3DSH sensor over SPI interface. More information about Zephyr SPI interfacing can be found at [our blog post](https://www.oxeltech.de/en/blogs/using-zephyr-os-for-interfacing-an-imu-sensor-with-nrf52-over-spi)
  
  
  
 ## Getting Started



Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html). First step, you should clone the example-application from [Example-Application](https://github.com/zephyrproject-rtos/example-application). Follow the instructions on the example-application page to initialise a workspace. After this  you can follow detailed instructions and guidelines on our blog [link](https://www.oxeltech.de/en/blogs) on how to setup your config and driver files. You can download individual files and folders from this repository and replace them with the default ones inside the example-application.



## Steps

1.	Copy and replace this repository's "drivers" folder inside "example-application/."
2.	Clone and add the st,lis3dsh.yaml file to the dts/bindings/sensor directory in the example-application project
3.	Clone and add the nrf52dk_nrf52832.overlay file to example-application/app/boards directory. 
4.	Add samples/sensor/lis3dsh/prj.conf from this repository to example-application/app/
5.	Clone and replace the main.c source code to the example-application/app/src folder.



### Build & Run

The application can be built and flashed by running the following commands within the example-application folder:

```code
west build -b nrf52dk_nrf52832 app
```

```code
west flash
```
