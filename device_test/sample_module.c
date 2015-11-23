#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
static int sample_module_init(void)
{
	printk(KERN_ALERT"sample_module register\n");
	printk(KERN_INFO "the process is \"%s\" (pid %i)\n", current->comm, current->pid);

	struct cdev *my_dev = cdev_alloc();
	my_dev->ops = &my_fops;
	return 0;
}
static void sample_module_cleanup(void)
{
	printk(KERN_ALERT"sample_module unregister\n");
}
module_init(sample_module_init);
module_exit(sample_module_cleanup);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cuizhl");

