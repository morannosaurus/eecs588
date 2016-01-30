ccflags-y := -std=gnu99
obj-m += attack_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

getdents: getdents.c
	gcc getdents.c -g -o getdents

payload: payload.cpp
	g++ -Wno-write-strings payload.cpp -g -o payload

deactivate: deactivate.cpp
	g++ -Wno-write-strings deactivate.cpp -g -o deactivate
