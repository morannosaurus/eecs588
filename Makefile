ccflags-y := -std=gnu99
obj-m += attack_module.o

all: payload payload-demo
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo chown root "attack_module.ko"
	sudo chmod 777 "attack_module.ko"
	sudo chmod u+s "attack_module.ko"
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f *.ko
	rm -f *.o
	rm -f *.mod.c
	rm -f payload-demo
	rm -f payload

getdents: getdents.c
	gcc getdents.c -g -o getdents
	sudo chown root "payload"
	sudo chmod 777 "payload"
	sudo chmod u+s "payload"

payload: payload.cpp
	g++ -Wno-write-strings payload.cpp -g -o payload

payload-demo: payload-demo.cpp
	g++ -Wno-write-strings payload-demo.cpp -g -o payload-demo
	sudo chown root "payload-demo"
	sudo chmod 777 "payload-demo"
	sudo chmod u+s "payload-demo"

deactivate: deactivate.cpp
	g++ -Wno-write-strings deactivate.cpp -g -o deactivate
