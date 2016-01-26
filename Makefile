ccflags-y := -std=gnu99
obj-m += hello1.o
obj-m += hello2.o
obj-m += hello3.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insmod:
	sudo insmod hello2.ko arg="foo"
