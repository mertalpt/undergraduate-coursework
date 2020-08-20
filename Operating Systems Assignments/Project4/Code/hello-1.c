/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_1_init(void)
{
	printk(KERN_INFO "Hello, world!\n");
	return 0;
}

static void __exit hello_1_exit(void)
{
	printk(KERN_INFO "Goodbye, world!\n");
}

MODULE_LICENSE("Dual BSD/GPL");
module_init(hello_1_init);
module_exit(hello_1_exit);
