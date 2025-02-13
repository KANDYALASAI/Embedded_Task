#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

dev_t dev=0;

static int __init entry_fun(void)
{
	if(alloc_chrdev_region(&dev,0,1,"sai kumar")<0)
	{
		printk("KERN_INFO Cant allocate a major number for this \n");
		return -1;
	}
	printk("MAJOR=%d MINOR=%d \n",MAJOR(dev),MINOR(dev));
	printk("Kernel Module was inserted successfull \n");
	return 0;
}

static void __exit remove_fun(void)
{
	unregister_chrdev_region(dev,1);
	printk("KERN_INFO mdoule was removed successfull \n");
}
module_init(entry_fun);
module_exit(remove_fun);

MODULE_AUTHOR("SAI KUMAR");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DYNAMICALL ALLOCATION FOR MAJOR AND MINOR NUMBER ");
