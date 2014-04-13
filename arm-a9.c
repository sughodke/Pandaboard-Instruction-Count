/*  
 *  hello-2.c - Demonstrating the module_init() and module_exit() macros.
 *  This is preferred over using init_module() and cleanup_module().
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

static int __init hello_2_init(void)
{
	printk(KERN_INFO "Sending ARM specific perf commands\n");

	/* enable user-mode access to the performance counter*/
	asm ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));

	/* disable counter overflow interrupts (just in case)*/
	asm ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));

	return 0;
}

static void __exit hello_2_exit(void)
{
	printk(KERN_INFO "Goodbye ARM \n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
