#include "include.h"
#include "rw.h"
#include "utility.h"
#include "syscall.h"
MODULE_LICENSE("GPL");

//char* arg;
//module_param(arg, charp, 0);

int (*orig_syscall)(const char *path); /*the original systemcall*/

int replace_syscall(char* path) {
	printk("BEGIN REPLACEMENT CALL");
	char* extended_directory_path = concat(path, "_fun_time\n");
	path[0] = 'Z';
	int orig_call_ret_code = orig_syscall(path);
	char* c;
	char* d;
	c = concat(KERN_INFO, path);
	d = concat(c, "\n");
	kfree(c);
	printk(d);
	printk("original_call_path: %s", path);
	printk("original_call_return_code: %d", orig_call_ret_code);
	kfree(extended_directory_path);
	kfree(d);
	printk("END REPLACEMENT CALL");
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
