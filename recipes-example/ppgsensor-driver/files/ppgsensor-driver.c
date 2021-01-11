#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include "data.h"

#define NSAMPLES 2048

static dev_t ppgsensor_dev;

struct cdev ppgsensor_cdev;

struct class *myclass = NULL;

static char buffer[64];

static int index; //index of ppg vector (use offset instead of index)

ssize_t ppgsensor_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int ppgdata;
    ppgdata = ppg[index];
    printk(KERN_INFO "Current vector value: %d\n", ppgdata);		//TODO REMOVE
    printk(KERN_INFO "Index value BEFORE updating = %d\n", index);	//TODO REMOVE
    index = (index + 1) % NSAMPLES;
    printk(KERN_INFO "Index value AFTER updating = %d\n", index);	//TODO REMOVE
    if ((copy_to_user((int*) buf, &ppgdata, count)) != 0) {  
	printk(KERN_ERR "[ppgsensor] not read correctly!\n");
	}
    return count;
}

int ppgsensor_open(struct inode *i, struct file *filp)
{
    printk(KERN_INFO "[ppgsensor] has been accessed correctly\n");
    printk(KERN_INFO "Not initialized index value = %d\n", index); 	//TODO REMOVE
    index = 0;
    printk(KERN_INFO "Current index value = %d\n", index); 	//TODO REMOVE
    printk(KERN_INFO "NSAMPLES = %d\n", NSAMPLES);		//TODO REMOVE
    return 0;
}


struct file_operations ppgsensor_fops = {
    .owner = THIS_MODULE,
    .open = ppgsensor_open,
    .read = ppgsensor_read,
};

static int __init ppgsensor_module_init(void)
{
    printk(KERN_INFO "Loading ppgsensor_module\n");

    alloc_chrdev_region(&ppgsensor_dev, 0, 1, "ppgsensor_dev");
    printk(KERN_INFO "%s\n", format_dev_t(buffer, ppgsensor_dev));

    myclass = class_create(THIS_MODULE, "ppgsensor_sys");
    device_create(myclass, NULL, ppgsensor_dev, NULL, "ppgsensor_dev");

    cdev_init(&ppgsensor_cdev, &ppgsensor_fops);
    ppgsensor_cdev.owner = THIS_MODULE;
    cdev_add(&ppgsensor_cdev, ppgsensor_dev, 1);

    return 0;
}

static void __exit ppgsensor_module_cleanup(void)
{
    printk(KERN_INFO "Cleaning-up ppgsensor_dev.\n");

    device_destroy(myclass, ppgsensor_dev );
    cdev_del(&ppgsensor_cdev);
    class_destroy(myclass);
    unregister_chrdev_region(ppgsensor_dev, 1);
}

module_init(ppgsensor_module_init);
module_exit(ppgsensor_module_cleanup);

MODULE_AUTHOR("Leonardo Palmucci");
MODULE_LICENSE("GPL");

