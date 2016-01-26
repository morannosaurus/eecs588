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

#endif