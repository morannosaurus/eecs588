#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
char* arg;
module_param(arg, charp, 0);

//concatenate two strings together
//user is responsible for kfreeing returned memory
char* concat(char* left, char* right) {
	int leftlen = strlen(left);
	int rightlen = strlen(right);
	char* full = kmalloc(leftlen+rightlen+1, GFP_KERNEL);
	strcpy(full, left);
	strcpy(full+leftlen, right);
	return full;
}

int init_module() {
	char* c;
	char* d;
	printk(KERN_INFO "Hello world\n");
	c = concat(KERN_INFO, arg);
	d = concat(c, "\n");
	kfree(c);
	printk(d);
	kfree(c);
	return 0;
}

void cleanup_module() {
	printk(KERN_INFO "Goodbye world\n");
}
