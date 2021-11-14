obj-m+=oscar_cubelesP1.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
install:
	sudo insmod oscar_cubelesP1.ko;  cp -r buttonScripts  $(HOME) 
uninstall:
	sudo rmmod oscar_cubelesP1.ko


