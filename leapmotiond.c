//leapmotiond.c
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

//module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Princeton Ferro <http://gitlhub.com/Prince781>");
MODULE_DESCRIPTION("Provides minimal Linux support for the Leap Motion sensor.");

static dev_t first; //first device number
static struct cdev c_dev; //character device - Leap Motion
static struct class *cl; //device class
char *devName = "";
int times_opened = 0;

static int leapd_open(struct inode *inod, struct file *fil) {
	printk(KERN_INFO "Leap Motion accessed at interval %d.\n", ++times_opened);
	return 0;
}

static ssize_t leapd_read(struct file *filp, char *buff, size_t len, loff_t *off) {
	printk(KERN_INFO "Leap Motion is being read.\n");
	return 0;
}

static ssize_t leapd_write(struct file *filp,const char *buff,size_t len,loff_t *off) {
	printk(KERN_INFO "Leap Motion is being written to.\n");
	return len;
}

static int leapd_rls(struct inode * indo, struct file *fil) {
	printk(KERN_INFO "Leap Motion released.\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = leapd_open,
	.release = leapd_rls,
	.read = leapd_read,
	.write = leapd_write
};

static int __init leapmotiond_init(void) {
	printk(KERN_INFO "leapmotiond registered.");
	if (alloc_chrdev_region(&first, 81, 1, "Leap Dev Kit") < 0)
		return -1;
	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
		unregister_chrdev_region(first, 1);
		return -1;
	}
	if (device_create(cl, NULL, first, NULL, "leap_null") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, first, 1) == -1) {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	return 0;
}

static void __exit leapmotiond_exit(void) {
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1); //unregister device
	printk(KERN_INFO "leapmotiond unregistered.");
}

module_init(leapmotiond_init);
module_exit(leapmotiond_exit);
