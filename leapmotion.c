/********************************************
 * LeapFrog
 * ========
 * An open-source Linux character driver for
 * the Leap Motion sensor.
 * --------------------------------
 * 2013 Princeton Ferro
*********************************************/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define MODULE_NAME "leapmotion"
#define LEAP_VENDOR_ID 0xf182
#define LEAP_PRODUCT_ID 0x0003

static struct usb_device *device;
static dev_t leapdev; //leap device number
static struct cdev c_dev; //leap device structure
static struct class *cl; //leap device class

//basic plug function
static int leapmotion_probe(struct usb_interface *interface, const struct usb_device_id *id) {
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i;
	
	iface_desc = interface->cur_altsetting;
	printk(KERN_INFO "%s: Leap Motion interface %d now probed at address (%04X:%04X).\n", MODULE_NAME, iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
		endpoint = &iface_desc->endpoint[i].desc;
		printk(KERN_INFO "%s: ED[%d]->bEndpointAddress: 0x%02X\n", MODULE_NAME, i, endpoint->bEndpointAddress);
	}
	
	device = interface_to_usbdev(interface);
	return 0;
}

//basic unplug function
static void leapmotion_disconnect(struct usb_interface *interface) {
	struct usb_host_interface *iface_desc;
	iface_desc = interface->cur_altsetting;
	printk(KERN_INFO "%s: Leap Motion interface %d removed.\n", MODULE_NAME, iface_desc->desc.bInterfaceNumber);
}

// table of device IDs
static struct usb_device_id leapmotion_ids[] = {
	{ USB_DEVICE(LEAP_VENDOR_ID, LEAP_PRODUCT_ID) },
	{ } /* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, leapmotion_ids);

static struct usb_driver leapmotion_driver = {
	.name = MODULE_NAME,
	.id_table = leapmotion_ids,
	.probe = leapmotion_probe,
	.disconnect = leapmotion_disconnect
};

/*** BASIC FILE OPERATIONS IN /dev/leapmotion ***/

static int leapmotion_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "Opening /dev/leapmotion\n");
	return 0;
}

static int leapmotion_close(struct inode *i, struct file *f) {
	printk(KERN_INFO "Closing /dev/leapmotion\n");
	return 0;
}

static ssize_t leapmotion_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "Reading /dev/leapmotion\n");
	return 0;
}

static ssize_t leapmotion_write(struct file *f, const char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "Writing to /dev/leapmotion\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = leapmotion_open,
	.release = leapmotion_close,
	.read = leapmotion_read,
	.write = leapmotion_write
};

static int __init leapmotion_init(void) { //on module load
	printk("%s: Registered LeapMotion driver.\n", MODULE_NAME);
	if (usb_register(&leapmotion_driver) < 0)
		return -1;
	if (alloc_chrdev_region(&leapdev, 150, 1, "Leap Dev Kit") < 0)
		return -1;
	if ((cl = class_create(THIS_MODULE,"chardrv")) == NULL) {
		unregister_chrdev_region(leapdev, 1);
		return -1;
	}
	if (device_create(cl, NULL, leapdev, NULL, "leapmotion") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(leapdev, 1);
		return -1;
	}
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, leapdev, 1) == -1) {
		device_destroy(cl, leapdev);
		class_destroy(cl);
		unregister_chrdev_region(leapdev, 1);
		return -1;
	}
	return 0;
}

static void __exit leapmotion_exit(void) { //on module deregister
	cdev_del(&c_dev); //delete character driver
	device_destroy(cl, leapdev);
	class_destroy(cl);
	unregister_chrdev_region(leapdev, 1);
	usb_deregister(&leapmotion_driver);
}

// module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Princeton Ferro <http://github.com/Prince781>");
MODULE_DESCRIPTION("An open-source Linux character driver for the Leap Motion sensor.");

module_init(leapmotion_init);
module_exit(leapmotion_exit);
