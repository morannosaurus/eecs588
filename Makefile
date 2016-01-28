ccflags-y := -std=gnu99
obj-m += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

getdents: getdents.c
	gcc getdents.c -g -o getdents

payload: payload.cpp
	g++ payload.cpp -g -o payload

