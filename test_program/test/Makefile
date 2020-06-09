DEV	:= test
FND	:= fpga_fnd_driver
SWT	:= fpga_push_switch_driver
APP	:= fpga_$(DEV)_test
TFTPDIR := /tftpboot
CC 	= arm-none-linux-gnueabi-gcc
KDIR	:= /root/work/achroimx6q/kernel
PWD	:= $(shell pwd)
obj-m	:= $(FND).o $(SWT).o

all:	$(FND) $(SWT) $(APP)

$(FND):	$(FND).c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(SWT):	$(SWT).c
	$(MAKE)	-C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(APP): $(APP).c
	$(CC) -o $(APP) $(APP).c

install:
	cp -a .ko $(APP) $(TFTPDIR)

clean:
	rm -rf *.ko *.mod.* *.order *.symvers *.o $(APP)

new:
	$(MAKE) clean
	$(MAKE)
