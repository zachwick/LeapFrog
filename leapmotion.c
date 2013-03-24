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

#define MODULE_NAME "leapmotion"

// module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Princeton Ferro <http://github.com/Prince781>");
MODULE_DESCRIPTION("An open-source Linux character driver for the Leap Motion sensor.");

static int leapmotion_probe(struct usb_interface *interface, const struct usb_device_id *id) {
	printk(KERN_INFO "%s: Leap Motion (%04X:%04X) plugged in.\n", MODULE_NAME, id->idVendor, id->idProduct);
	return 0;
}

static void leapmotion_disconnect(struct usb_interface *interface) {
	printk(KERN_INFO "%s: Leap Motion removed.\n", MODULE_NAME);
}

static struct usb_device_id leapmotion_table[] = { //device id information for Leap Kits
	{ USB_DEVICE(0xf182, 0x0003) },
	{} //terminating entry
};

MODULE_DEVICE_TABLE(usb, leapmotion_table);

static struct usb_driver leapmotion = {
	.name = MODULE_NAME,
	.id_table = leapmotion_table,
	.probe = leapmotion_probe,
	.disconnect = leapmotion_disconnect,
};

static int __init leapmotion_init(void) { //on module load
	return usb_register(&leapmotion);
}

static void __exit leapmotion_exit(void) { //on module deregister
	usb_deregister(&leapmotion);
}

module_init(leapmotion_init);
module_exit(leapmotion_exit);
