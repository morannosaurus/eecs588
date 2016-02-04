#ifndef UTILITY_H
#define UTILITY_H
#include "include.h"
//concatenate two strings together
//user is responsible for kfreeing returned memory
char* concat(char* left, char* right) {
	int leftlen = strlen(left);
	int rightlen = strlen(right);
	char* full = kmalloc(leftlen + rightlen + 1, GFP_KERNEL);
	strcpy(full, left);
	strcpy(full + leftlen, right);
	return full;
}

char* strnstrn(char* haystack, int haystack_len, char* needle, int needle_len) {
	int found_bytes;
	int i;
	char* found_start;
	found_bytes = 0;
	found_start = 0;
	for (i = 0; i < haystack_len; ++i) {
		if (haystack[i] == needle[found_bytes]) {
			if (found_bytes == 0) {
				found_start = haystack + i;
			}
			found_bytes++;
			if (found_bytes == needle_len) {
				return found_start;
			}
		}
		else {
			found_bytes = 0;
			found_start = 0;
		}
	}
	return 0;
}

#endif