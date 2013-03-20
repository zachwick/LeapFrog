//leapmotiond.c
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static dev_t first; //first device number
char *devName = "";
static int __init leapmotiond_init(void) {
	printk(KERN_INFO "leapmotiond registered.");
	if (alloc_chrdev_region(&first, 0, 3, "Leap Dev Kit") < 0)
		return -1;
	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n". MAJOR(first), MINOR(first)); //debug
	return 0;
}

static void __exit leapmotiond_exit(void) {
	unregister_chrdev_region(first, 3); //unregister device
	printk(KERN_INFO "leapmotiond unregistered.");
}

module_init(leapmotiond_init);
module_exit(leapmotiond_exit);

MODULE_LICENSE("NONE");
MODULE_AUTHOR("Princeton Ferro <http://gitlhub.com/Prince781>");
MODULE_DESCRIPTION("Provides minimal Linux support for the Leap Motion sensor.");
