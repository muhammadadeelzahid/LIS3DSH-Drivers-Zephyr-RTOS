/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>

#include "app_version.h"

void main(void)
{
    //const struct device *sensor = device_get_binding (DT_LABEL (DT_INST (0, st_lis3dsh)));
    //const struct device *sensor = DEVICE_DT_GET(DT_INST(0,st_lis3dsh));

    const struct device *sensor = DEVICE_DT_GET(DT_INST(0,st_lis3dsh));

    if (sensor == NULL)
    {
        printk("Could not get the device \r\n");
    }
    if (!device_is_ready(sensor))
    {
        printk("Device not ready \r\n");
    }
    //require API calls from Sensors API 

    while(1)
    {
        struct sensor_value acc_x,acc_y,acc_z;
        sensor_sample_fetch(sensor);
        sensor_channel_get(sensor,SENSOR_CHAN_ACCEL_X,&acc_x);
        sensor_channel_get(sensor,SENSOR_CHAN_ACCEL_Y ,&acc_y);
        sensor_channel_get(sensor,SENSOR_CHAN_ACCEL_Z ,&acc_z);
        printk("X: %dmg\r\n",acc_x.val1);
        printk("Y: %dmg\r\n",acc_y.val1);
        printk("Z: %dmg\r\n",acc_z.val1);
        printk("\r\n");
        k_msleep(500);

    }

}

