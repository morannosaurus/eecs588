ccflags-y := -std=gnu99
obj-m += hello1.o
obj-m += hello2.o
obj-m += hello3.o
obj-m += hello_kernel.o
obj-m += mkdir_blocker.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

