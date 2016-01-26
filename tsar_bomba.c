#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
MODULE_LICENSE("GPL");

//char* arg;
//module_param(arg, charp, 0);

SYS_mkdir_type orig_syscall; /*the original systemcall*/ 

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
	orig_syscall = (SYS_mkdir_type) sys_call_table[SYS_fork];
	sys_call_table[SYS_fork] = (syscall) replace_syscall;
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_fork] = (syscall) orig_syscall;
	printk(KERN_INFO "Module Removed\n");
}
