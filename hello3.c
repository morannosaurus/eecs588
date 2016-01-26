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

#define SYS_mkdir 83

//char* arg;
//module_param(arg, charp, 0);

void** sys_call_table = 0xffffffff81801460;

int (*orig_syscall)(const char *path); /*the original systemcall*/ 


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

int replace_syscall(char* path) {
	char* c;
	char* d;
	c = concat(KERN_INFO, path);
	d = concat(c, "\n");
	kfree(c);
	printk(d);
	kfree(d);
	return 0;
}

int init_module() {
	orig_syscall = sys_call_table[SYS_mkdir];
	sys_call_table[SYS_mkdir] = replace_syscall;
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_mkdir] = orig_syscall;
	printk(KERN_INFO "Goodbye world\n");
}
