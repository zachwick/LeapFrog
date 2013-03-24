# Makefile for LeapFrog's leapmotion.ko
obj-m = leapmotion.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
