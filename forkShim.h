//this stuff doesn't work :(

#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "secrets.h"
MODULE_LICENSE("GPL");

int forkShim(void) {
	printk(KERN_INFO "%s\n", "fork called");
	return ((SYS_fork_type)backup_sys_call_table[SYS_fork])();
}

asmlinkage long cloneShim(unsigned long a, unsigned long b, int __user * c, int d,
	int __user * e)
{
	//printk(KERN_INFO "%s\n", "clone called");
	return ((SYS_clone_type)backup_sys_call_table[SYS_clone])(a, b, c, d, e);
}

