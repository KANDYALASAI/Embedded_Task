
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include<linux/cdev.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev simple_cdev;

/*
 ** Function Prototypes
 */
static int   __init simple_driver_init(void);
static void  __exit simple_driver_exit(void);
int  simple_open(struct inode *inode, struct file *file);
int simple_release(struct inode *inode, struct file *file);
ssize_t simple_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
ssize_t  simple_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = simple_read,
    .write      = simple_write,
    .open       = simple_open,
    .release    = simple_release,
};
/*
** This function will be called when we open the Device file
*/
 int simple_open(struct inode *inode, struct file *file)
{
        pr_info("Driver Open Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/
int simple_release(struct inode *inode, struct file *file)
{
        pr_info("Driver Release Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
ssize_t simple_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Read Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
 ssize_t simple_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Write Function Called...!!!\n");
        return len;
}
/*
** Module init function
*/
static int __init simple_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "simple_char")) <0){
                pr_info("Cannot allocate major number for device\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
	 /*Creating cdev structure*/
        cdev_init(&simple_cdev,&fops);
        /*Adding character device to the system*/
        if((cdev_add(&simple_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
        /*Creating struct class*/
        dev_class = class_create("simple_class");
        if(IS_ERR(dev_class)){
            pr_info("Cannot create the struct class for device\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"simple_device"))){
            pr_info("Cannot create the Device\n");
            goto r_device;
        }
        printk("Kernel Module Inserted Successfully...\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
 
/*
** Module exit function
*/
static void __exit simple_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
	cdev_del(&simple_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(simple_driver_init);
module_exit(simple_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sai kumar");
MODULE_DESCRIPTION("Simple linux driver (Automatically Creating a Device file)");
