#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_License("Dual BSD/GPL");

static int my_xtime_init(void){
	printk(KERN_ALERT "Xtime Initialized!\n");
}

static int my_xtime_exit(void){
	printl(KERN_ALERT "Xtime Exit!\n");
}

module_init(my_xtime_init);
module_exit(my_xtime_exit);