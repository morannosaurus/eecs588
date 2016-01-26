ccflags-y := -std=gnu99
obj-m += hello3.o
obj-m += hello_kernel.o
obj-m += mkdir_blocker.o
obj-m += tsar_bomba.o
obj-m += k_getdents.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

getdents: getdents.c
	gcc getdents.c -g -o getdents
