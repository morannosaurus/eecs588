#include "include.h"
#include "rw.h"
#include "utility.h"
#include "syscall.h"
MODULE_LICENSE("GPL");

SYS_mkdir_type orig_syscall; /*the original systemcall*/ 

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
	make_rw(sys_call_table);
	orig_syscall = (SYS_mkdir_type)sys_call_table[SYS_mkdir];
	sys_call_table[SYS_mkdir] = (syscall)replace_syscall;
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_mkdir] = (syscall)orig_syscall;
	printk(KERN_INFO "Goodbye world\n");
}
