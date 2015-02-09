#include <linux/module.h> //indicate for all kernel modules
#include <linux/kernel.h> // include for KERN_INFO
#include <linux/init.h> //include for __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JERRY");
MODULE_DESCRIPTION("A Simple Hello World module");

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello World!\n");
	return 0; //non-zero return means that the module could't be loaded.
}

static void __exit hello_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
