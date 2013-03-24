/********************************************
 * LeapFrog
 * ========
 * An open-source Linux character driver for
 * the Leap Motion sensor.
 * --------------------------------
 * 2013 Princeton Ferro
*********************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define MODULE_NAME "leapmotion"
#define LEAP_VENDOR_ID 0xf182
#define LEAP_PRODUCT_ID 0x0003

static struct usb_device *device;
static struct usb_class_driver class;

/*** BASIC FILE OPERATIONS IN /dev/leapmotion ***/

static int leapmotion_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "%s: Opening /dev/leapmotion\n", MODULE_NAME);
	return 0;
}

static int leapmotion_close(struct inode *i, struct file *f) {
	printk(KERN_INFO "%s: Closing /dev/leapmotion\n", MODULE_NAME);
	return 0;
}

static ssize_t leapmotion_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "%s: Reading /dev/leapmotion\n", MODULE_NAME);
	return 0;
}

static ssize_t leapmotion_write(struct file *f, const char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "%s: Writing to /dev/leapmotion\n", MODULE_NAME);
	return 0;
}

static struct file_operations fops = {
	.open = leapmotion_open,
	.release = leapmotion_close,
	.read = leapmotion_read,
	.write = leapmotion_write
};

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
	
	int retval;
	
	class.name = "usb/leapmotion%d";
	class.fops = &fops;
	if ((retval = usb_register_dev(interface, &class)) < 0)
		printk(KERN_INFO "%s: Unable to get a minor for this device.", MODULE_NAME);
	else printk(KERN_INFO "%s: Minor obtained: %d\n", MODULE_NAME, interface->minor);
	
	return retval;
}

//basic unplug function
static void leapmotion_disconnect(struct usb_interface *interface) {
	struct usb_host_interface *iface_desc;
	iface_desc = interface->cur_altsetting;
	printk(KERN_INFO "%s: Leap Motion interface %d removed.\n", MODULE_NAME, iface_desc->desc.bInterfaceNumber);
	usb_deregister_dev(interface, &class);
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

static int __init leapmotion_init(void) { //on module load
	return usb_register(&leapmotion_driver); //register with USB subsystem
}

static void __exit leapmotion_exit(void) { //on module deregister
	usb_deregister(&leapmotion_driver); //deregister with USB subsystem
}

// module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Princeton Ferro <http://github.com/Prince781>");
MODULE_DESCRIPTION("An open-source Linux character driver for the Leap Motion sensor.");

module_init(leapmotion_init);
module_exit(leapmotion_exit);
