obj-m += lkm1.o lkm2.o lkm3.o lkm4.o

 
PWD := $(CURDIR) 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean