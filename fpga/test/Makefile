DEV	:= test
FND	:= fpga_fnd_driver
SWT	:= fpga_push_switch_driver
MT 	:= fpga_step_motor_driver
BZ	:= fpga_buzzer_driver
TX	:= fpga_text_lcd_driver
DIP	:= fpga_dip_switch_driver
APP	:= fpga_$(DEV)_test
TFTPDIR := /tftpboot
CC 	= arm-none-linux-gnueabi-gcc
KDIR	:= /root/work/achroimx6q/kernel
PWD	:= $(shell pwd)
obj-m	:= $(FND).o $(SWT).o $(MT).o $(BZ).o $(TX).o $(DIP).o

all:	$(FND) $(SWT) $(MT) $(BZ) $(TX) $(DIP) $(APP)

$(FND):	$(FND).c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(SWT):	$(SWT).c
	$(MAKE)	-C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(MT):	$(MT).c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(BZ):	$(BZ).c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(TX):	$(TX).c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(DIP):	$(DIP).c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

$(APP): $(APP).c
	$(CC) -o $(APP) $(APP).c

install:
	cp -a .ko $(APP) $(TFTPDIR)

clean:
	rm -rf *.ko *.mod.* *.order *.symvers *.o $(APP)

new:
	$(MAKE) clean
	$(MAKE)
