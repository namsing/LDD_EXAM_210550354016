obj-m:=chardev.o
KDIR=/lib/modules/$(shell uname -r)/build/

all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o App1 App1.c
	gcc -o App2 App2.c
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf App2 App1

install:
	make -C $(KDIR) M=$(PWD) modules_install
