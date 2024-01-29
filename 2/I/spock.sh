#!/bin/bash

make -C /lib/modules/$(uname -r)/build M=$(pwd) modules

sudo insmod ioctl_driver.ko

gcc app.c -o app

./app

sudo rmmod ioctl_driver

make -C /lib/modules/$(uname -r)/build M=$(pwd) clean
rm app
