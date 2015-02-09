#
# For Kernel Module hello.c
#
TARGET = hello
KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
obj-m 	+= $(TARGET).o

default:
	make -C $(KDIR) M=$(PWD) modules
test:
	insmod ./hello.ko
	dmesg | tail -1
	rmmod ./hello.ko
	dmesg | tail -1
clean:
	make -C $(KDIR) M=$(PWD) clean
