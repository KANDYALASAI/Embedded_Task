/**
 * @file bmp280_driver.c
 * @brief Linux Character Driver for BMP280 Temperature Sensor using I2C
 * @author kandyala sai kumar
 * @date 2025
 *
 * This driver allows communication with the BMP280 temperature sensor
 * over the I2C interface in a Linux kernel environment.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>

#define DEVICE_NAME "bmp280"
#define BMP280_I2C_ADDR 0x76
#define BMP280_TEMP_MSB_REG 0xFA
#define BMP280_CTRL_MEAS_REG 0xF4
#define BMP280_RESET_REG 0xE0
#define BMP280_CALIB_START 0x88

static dev_t dev_num;
static struct cdev bmp280_cdev;
static struct class *bmp280_class;
static struct i2c_adapter *adapter;

/* Structure to store BMP280 calibration data */
struct bmp280_calib_data {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
};

static struct bmp280_calib_data calib;

/**
 * @brief Performs a soft reset on the BMP280 sensor
 */
static int bmp280_soft_reset(void)
{
    uint8_t reset_cmd[2] = {BMP280_RESET_REG, 0xB6};
    struct i2c_msg msg = {BMP280_I2C_ADDR, 0, 2, reset_cmd};

    if (i2c_transfer(adapter, &msg, 1) < 0) {
        pr_err("BMP280: Failed to reset sensor\n");
        return -EIO;
    }

    msleep(2); // Wait for sensor to reset
    return 0;
}

/**
 * @brief Configures the BMP280 sensor for normal operation
 */
static int bmp280_configure(void)
{
    uint8_t config_cmd[2] = {BMP280_CTRL_MEAS_REG, 0x27}; // Normal mode, temp oversampling x1
    struct i2c_msg msg = {BMP280_I2C_ADDR, 0, 2, config_cmd};

    if (i2c_transfer(adapter, &msg, 1) < 0) {
        pr_err("BMP280: Failed to configure sensor mode\n");
        return -EIO;
    }

    return 0;
}

/**
 * @brief Reads BMP280 calibration data
 * @return int 0 on success, negative error code on failure
 */
static int bmp280_read_calibration(void)
{
    uint8_t calib_data[6];
    struct i2c_msg msgs[2] = {
        {BMP280_I2C_ADDR, 0, 1, calib_data}, // Send calibration register address
        {BMP280_I2C_ADDR, I2C_M_RD, 6, calib_data} // Read 6 bytes of calibration data
    };

    calib_data[0] = BMP280_CALIB_START;

    if (i2c_transfer(adapter, msgs, 2) < 0) {
        pr_err("BMP280: Failed to read calibration data\n");
        return -EIO;
    }

    // Store calibration values
    calib.dig_T1 = (calib_data[1] << 8) | calib_data[0];
    calib.dig_T2 = (calib_data[3] << 8) | calib_data[2];
    calib.dig_T3 = (calib_data[5] << 8) | calib_data[4];

    pr_info("Calibration Data: T1=%u, T2=%d, T3=%d\n", calib.dig_T1, calib.dig_T2, calib.dig_T3);

    return 0;
}

/**
 * @brief Converts raw temperature data to Celsius
 * @param raw_temp Raw temperature data
 * @return int32_t Temperature in Celsius * 100
 */
static int32_t bmp280_compensate_temperature(int32_t raw_temp)
{
    int32_t var1, var2, temp;

    var1 = ((((raw_temp >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
    var2 = (((((raw_temp >> 4) - ((int32_t)calib.dig_T1)) * ((raw_temp >> 4) - ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;

    temp = var1 + var2;
    return (temp * 5 + 128) >> 8;
}

/**
 * @brief Reads and converts temperature data from BMP280 sensor
 * @return int32_t Temperature in Celsius * 100
 */
static int32_t bmp280_read_temperature(void)
{
    uint8_t data[3] = {0};
    int32_t temp_raw = 0;
    struct i2c_msg msgs[2] = {
        {BMP280_I2C_ADDR, 0, 1, data},
        {BMP280_I2C_ADDR, I2C_M_RD, 3, data}
    };

    data[0] = BMP280_TEMP_MSB_REG;

    if (i2c_transfer(adapter, msgs, 2) < 0) {
        pr_err("BMP280: Failed to read temperature data\n");
        return -EIO;
    }

    pr_info("Raw Temperature Data: 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2]);

    temp_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    return bmp280_compensate_temperature(temp_raw);
}

/* File operations structure for the character device */
static ssize_t bmp280_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    int32_t temperature;
    char temp_buf[32];

    temperature = bmp280_read_temperature();
    if (temperature < 0) {
        return temperature;
    }

    snprintf(temp_buf, sizeof(temp_buf), "%d\n", temperature);
    if (copy_to_user(buf, temp_buf, strlen(temp_buf))) {
        return -EFAULT;
    }

    return strlen(temp_buf);
}

/* File operations for BMP280 driver */
static struct file_operations bmp280_fops = {
    .owner = THIS_MODULE,
    .read = bmp280_read,
};

/**
 * @brief Initializes the BMP280 driver
 */
static int __init bmp280_init(void)
{
    int ret;

    adapter = i2c_get_adapter(1);
    if (!adapter) {
        pr_err("BMP280: Failed to acquire I2C adapter\n");
        return -ENODEV;
    }

    bmp280_soft_reset(); // to reset the bmp280
    msleep(10);

    bmp280_configure();
    bmp280_read_calibration();

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&bmp280_cdev, &bmp280_fops);
    cdev_add(&bmp280_cdev, dev_num, 1);

    bmp280_class = class_create(DEVICE_NAME);
    device_create(bmp280_class, NULL, dev_num, NULL, DEVICE_NAME);

    pr_info("BMP280: Device initialized successfully\n");
    return 0;
}

/**
 * @brief Cleans up the BMP280 driver
 */
static void __exit bmp280_exit(void)
{
    device_destroy(bmp280_class, dev_num);
    class_destroy(bmp280_class);
    cdev_del(&bmp280_cdev);
    unregister_chrdev_region(dev_num, 1);
    i2c_put_adapter(adapter);
    pr_info("BMP280: Device removed\n");
}

module_init(bmp280_init);
module_exit(bmp280_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kandyala Sai Kumar");
MODULE_DESCRIPTION("Linux Character Driver for BMP280 Temperature Sensor");
MODULE_VERSION("1.2");

