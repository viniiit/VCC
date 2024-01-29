#!/bin/bash

make 

sudo insmod ioctl_dr_sold.ko

./control_station &
c_pid=$!
echo "central station PID: $c_pid"

./soldier $c_pid &
echo "Soldier PID: $!"

sleep 2
kill -9 $c_pid

sudo rmmod ioctl_dr_sold