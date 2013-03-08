//leapmotiond.c
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init leapmotiond_init(void) {
	printk(KERN_INFO "leapmotiond registered.");
	return 0;
}

static void __exit leapmotiond_exit(void) {
	printk(KERN_INFO "leapmotiond unregistered.");
}

module_init(leapmotiond_init);
module_exit(leapmotiond_exit);

MODULE_LICENSE("NONE");
MODULE_AUTHOR("Princeton Ferro <http://github.com/Prince781>");
MODULE_DESCRIPTION("Provides minimal Linux support for the LEAP Motion sensor.");
