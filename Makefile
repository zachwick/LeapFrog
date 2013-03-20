# Makefile - build out LeapMotionD 

# if KERNELRELEASE is defined, we can use the language,
# as we've been invoked from the kernel build system
ifneq (${KERNELRELEASE},)
	obj-m := leapmotiond.o
# otherwise, we were called directly from the command line
# invoke the kernel build system
else
	KERNEL_SOURCE := /usr/src/kernels/`uname -r`/   # rely on any installed kernel sources
	PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules
	
clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif

