#include "include.h"

int init_module() {
	printk(KERN_INFO "Hello world\n");
	return 0;
}

void cleanup_module() {
	printk(KERN_INFO "Goodbye world\n");
}
