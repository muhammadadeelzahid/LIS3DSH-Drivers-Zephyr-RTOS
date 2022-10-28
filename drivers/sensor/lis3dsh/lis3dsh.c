#define DT_DRV_COMPAT st_lis3dsh

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/spi.h>
#include "lis3dsh.h"


//Helper functions 
/* A function to convert two's complement value to 16-bit int value */
int twoComplToInt16(int twoComplValue)
{
	  int int16Value = 0;

	  /* conversion */
	  if(twoComplValue > 32768)
	  {
		    int16Value = -(((~twoComplValue) & 0xFFFF) + 1);
	  }
	  else
	  {
		    int16Value = twoComplValue;
	  }

	  return int16Value;
}

static void LIS3DSH_write_reg(const struct device *dev,int reg, int val)
{
    struct lis3dsh_data *data = dev->data;
    const struct lis3dsh_config *cfg = dev->config;



    SPIWriteLength = 2; // set the spi write length to 2 bytes
    SPIReadLength = 0; // set the read length

    data->spi_tx_buf[0] = SET_WRITE_SINGLE_CMD(reg); // set the first byte which is a write command
    data->spi_tx_buf[1] = val; // A byte of data to be sent
	
    //declare them in every read write function
    struct spi_buf tx_buf_arr; 
    struct spi_buf_set tx;
    struct spi_buf rx_buf_arr;
    struct spi_buf_set rx ;


   //specifying the tx and rx buffer specific to zephyr's SPI drivers
   	tx_buf_arr.buf = data->spi_tx_buf;
    tx_buf_arr.len = SPIWriteLength;
    tx.buffers = &tx_buf_arr;
    tx.count = 1;
	
    rx_buf_arr.buf = data->spi_rx_buf;
    rx_buf_arr.len = SPIReadLength;

    rx.buffers = &rx_buf_arr ;
    rx.count = 1;

	int error = spi_transceive_dt(&cfg->bus_cfg.spi, &tx, &rx);

	if(error != 0){
		printk("SPI transceive error: %i\n", error);
	}
   
}


int LIS3DSH_read_reg(const struct device *dev,int reg)
{	
    /* Set the read command for reading a single byte */

    struct lis3dsh_data *data = dev->data;
    const struct lis3dsh_config *cfg = dev->config;


    data->spi_tx_buf[0] = SET_READ_SINGLE_CMD(reg);  
  
    //declare them in every read write function
    struct spi_buf tx_buf_arr; 
    struct spi_buf_set tx;
    struct spi_buf rx_buf_arr;
    struct spi_buf_set rx ;



   //specifying the tx and rx buffer specific to zephyr's SPI drivers
   	tx_buf_arr.buf = data->spi_tx_buf;
    tx_buf_arr.len = 2;

    tx.buffers = &tx_buf_arr;
    tx.count = 1;
	
    rx_buf_arr.buf = data->spi_rx_buf;
    rx_buf_arr.len = 2;

    rx.buffers = &rx_buf_arr ;
    rx.count = 1;

	int error = spi_transceive_dt(&cfg->bus_cfg.spi, &tx, &rx);
	if(error != 0){
		printk("SPI transceive error: %i\n", error);
		return error;
	}

    return data->spi_rx_buf[1];
}


int SPI_init(const struct device *dev)
{

    struct lis3dsh_data *data = dev->data;
    const struct lis3dsh_config *cfg = dev->config;

    if(!spi_is_ready(&cfg->bus_cfg.spi)){
        printk("SPI bus is not ready \r\n");
    }

    
    return 0;
}


static int lis3dsh_init (const struct device *dev)
{
    const struct lis3dsh_config *cfg = dev->config;
    struct lis3dsh_data *data = dev->data;

    int status = cfg->bus_init(dev);
	if (status < 0) {
		return status;
	}
    LIS3DSH_chip_init(dev);

    return 0;
}



/* A function to initialize the Lis3dsh Sensor chip*/
int LIS3DSH_chip_init(const struct device *dev)
{        
    //initialising chip
    int regval;

    /* Read the who am i register to confirm lis3dsh is communicating with our nrf device */
    regval = LIS3DSH_read_reg(dev,ADD_REG_WHO_AM_I);

    /* check the who am i register value and compare it with default who am i value for lis3dsh 
        if who am i is correct then proced */
    if( regval == UC_WHO_AM_I_DEFAULT_VALUE )
    {
        LIS3DSH_write_reg(dev,ADD_REG_CTRL_4, UC_ADD_REG_CTRL_4_CFG_VALUE);

            /* verify written value */
        regval = LIS3DSH_read_reg(dev,ADD_REG_CTRL_4);

            /* if written value is different */
        if( regval != UC_ADD_REG_CTRL_4_CFG_VALUE )
        {
                printk("\r\nWrite Reg ERR\r\n");
                k_msleep(5000);
        }
    }
    else
    {
        printk("\nDevice does not exist\r\n");
        k_msleep(5000);
    }
    return 0;
}

static int lis3dsh_channel_get(const struct device *dev,
			      enum sensor_channel chan,
			      struct sensor_value *val)
{
    struct lis3dsh_data *dta = dev->data;
    
    switch(chan)
    {
        case SENSOR_CHAN_ACCEL_X:
            val->val1 = dta->x;
        break;
        case SENSOR_CHAN_ACCEL_Y:
            val->val1 = dta->y;

        break;
        case SENSOR_CHAN_ACCEL_Z:
            val->val1 = dta->z;
        break;
    }
}

static int get_acceleration( const struct device *dev)
{
    struct lis3dsh_data *dta = dev->data;
	const struct lis3dsh_config *cfg = dev->config;


    dta->x = ((LIS3DSH_read_reg(dev,ADD_REG_OUT_X_H) << 8) | LIS3DSH_read_reg(dev,ADD_REG_OUT_X_L));
    dta->y = ((LIS3DSH_read_reg(dev,ADD_REG_OUT_Y_H) << 8) | LIS3DSH_read_reg(dev,ADD_REG_OUT_Y_L));
    dta->z = ((LIS3DSH_read_reg(dev,ADD_REG_OUT_Z_H) << 8) | LIS3DSH_read_reg(dev,ADD_REG_OUT_Z_L));

    /* transform X value from two's complement to 16-bit int */
    dta->x = twoComplToInt16(dta->x);
    /* convert X absolute value to mg value */
    dta->x = dta->x * SENS_2G_RANGE_MG_PER_DIGIT;

    /* transform Y value from two's complement to 16-bit int */
    dta->y = twoComplToInt16(dta->y);
    /* convert Y absolute value to mg value */
    dta->y = dta->y * SENS_2G_RANGE_MG_PER_DIGIT;

    /* transform Z value from two's complement to 16-bit int */
    dta->z = twoComplToInt16(dta->z);
    /* convert Z absolute value to mg value */
    dta->z = dta->z * SENS_2G_RANGE_MG_PER_DIGIT;
    return 0;
}

static const struct sensor_driver_api lis3dsh_driver_api = {
    .channel_get = lis3dsh_channel_get,
    .sample_fetch = get_acceleration
};



//Register a node in the deviceTree 

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "LIS3DSH driver enabled without any devices"
#endif


#define LIS3DSH_DEVICE_INIT(inst)                                                                                                               \
        DEVICE_DT_INST_DEFINE (inst, lis3dsh_init, NULL, &lis3dsh_data_##inst, &lis3dsh_config_##inst, POST_KERNEL,                             \
                               CONFIG_SENSOR_INIT_PRIORITY, &lis3dsh_driver_api);

#define LIS3DSH_CONFIG_SPI(inst)						\
	{								\
		.bus_init = SPI_init,				\
		.bus_cfg = { .spi = SPI_DT_SPEC_INST_GET(inst,		\
					SPI_WORD_SET(8) |		\
					SPI_OP_MODE_MASTER |		\
					SPI_MODE_CPOL |			\
					SPI_MODE_CPHA,			\
					0) },				\
	}

#define LIS3DSH_DEFINE_SPI(inst)						\
	static struct lis3dsh_data lis3dsh_data_##inst;			\
	static const struct lis3dsh_config lis3dsh_config_##inst =	\
		LIS3DSH_CONFIG_SPI(inst);				\
	LIS3DSH_DEVICE_INIT(inst)


    DT_INST_FOREACH_STATUS_OKAY(LIS3DSH_DEFINE_SPI)
