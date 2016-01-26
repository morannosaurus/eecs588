#include <linux/module.h> 
#include <linux/kernel.h> 
#include <asm/unistd.h> 
#include <asm/fcntl.h> 
#include <asm/errno.h> 
#include <linux/types.h> 
#include <linux/dirent.h> 
#include <linux/string.h> 
#include <linux/fs.h> 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <asm/unistd.h>
#include "rw.h"
MODULE_LICENSE("GPL");
#include "syscall.h"

//char* arg;
//module_param(arg, charp, 0);

int (*orig_syscall)(const char *path); /*the original systemcall*/ 
const unsigned long** sys_call_table = (const unsigned long**)0xffffffff81801460;

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

// This kills the crab.
int replace_syscall(char* path) {
	printk("Bomba go");
	while(0)
		orig_syscall(path);	
	printk("END REPLACEMENT CALL");
	return 0;
}

int init_module() {
	make_rw(sys_call_table);
	orig_syscall = sys_call_table[SYS_fork];
	sys_call_table[SYS_fork] = replace_syscall;
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_fork] = orig_syscall;
	printk(KERN_INFO "Module Removed\n");
}
