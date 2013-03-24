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

char *modName = "leapmotiond"; //this is the module name

static dev_t first; //first device number
static struct cdev c_dev; //character device - Leap Motion
static struct class *cl; //device class
static char *copyData; //data to be copied
char *devName = ""; //device name
int num_leaps = -1; //current number of Leap Motion devices attached (-1 == 0)
int times_opened[4]; //we can assume a maximum of four devices attached

static int leapd_open(struct inode *inod, struct file *fil) {
	printk(KERN_INFO "%s: Leap Motion accessed at interval %d.\n", modName, ++times_opened);
	return 0;
}

static ssize_t leapd_read(struct file *filp, char *buff, size_t len, loff_t *off) {
	printk(KERN_INFO "%s: Leap Motion is being read.\n", modName);
	if (copy_to_user(buf, &copyData, 1) != 0) //copy buffer to user (reading)
		return -EFAULT;
	else return 1;
}

static ssize_t leapd_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	printk(KERN_INFO "%s: Leap Motion is being written to.\n", modName);
	if (copy_from_user(&copyData, buf+len-1, 1) != 0) //copy buffer to device (writing)
		return -EFAULT;
	else return len;
}

static int leapd_rls(struct inode * indo, struct file *fil) {
	printk(KERN_INFO "%s: Leap Motion released.\n", modName);
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
	printk(KERN_INFO "%s: registered.", modName);
	if (alloc_chrdev_region(&first, 81, 1, "Leap Dev Kit") < 0)
		return -1;
	if ((cl = class_create(THIS_MODULE, "leapfrog")) == NULL) {
		unregister_chrdev_region(first, 1);
		return -1;
	}
	if (device_create(cl, NULL, first, NULL, "leapmotion") == NULL) {
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
	printk(KERN_INFO "%s: unregistered.", modName);
}

module_init(leapmotiond_init);
module_exit(leapmotiond_exit);