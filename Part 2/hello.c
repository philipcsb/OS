#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <asm-generic/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NAMES");
MODULE_DESCRIPTION("Hello World Test Module");

#define ENTRY_NAME "hello_world"
#define PERMS 0644
#define PARENT NULL;

static struct file_operations fops;

static char *message;
static int read_p;

static int hello_init(void);
static void hello_exit(void);
int hello_proc_open(struct inode *sp_inode, struct file *sp_file);
ssize_t hello_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset);
int hello_proc_release(struct inode *sp_inode, struct file *sp_file);

static int hello_init(void) {
	printk("/proc/%s create\n", ENTRY_NAME); fops.open = hello_proc_open;
	fops.read = hello_proc_read;
	fops.release = hello_proc_release;
	if (!proc_create(ENTRY_NAME, PERMS, NULL, &fops)) { 
		printk("ERROR! proc_create\n"); 
		remove_proc_entry(ENTRY_NAME, NULL);
		return -ENOMEM;
	}
	return 0;
}

static void hello_exit(void) {
	remove_proc_entry(ENTRY_NAME, NULL);
	printk("Removing /proc/%s.\n", ENTRY_NAME);
} 


int hello_proc_open(struct inode *sp_inode, struct file *sp_file) { 
	printk("proc called open\n");
	read_p = 1;
	message = kmalloc(sizeof(char) * 20, __GFP_WAIT | __GFP_IO | __GFP_FS);
	if (message == NULL) {
		printk("ERROR, hello_proc_open");
		return -ENOMEM;
	}
	strcpy(message, "Hello, World!\n"); return 0;
}

ssize_t hello_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {
	int len = strlen(message);
	read_p = !read_p; 
	if (read_p) {
		return 0; }
	printk("proc called read\n"); copy_to_user(buf, message, len); return len;
}

int hello_proc_release(struct inode *sp_inode, struct file *sp_file) {
	printk("proc called release\n"); kfree(message);
	return 0;
}



module_init(hello_init);
module_exit(hello_exit);
