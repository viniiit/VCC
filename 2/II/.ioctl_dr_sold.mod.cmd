cmd_/home/cs695/2/II/ioctl_dr_sold.mod := printf '%s\n'   ioctl_dr_sold.o | awk '!x[$$0]++ { print("/home/cs695/2/II/"$$0) }' > /home/cs695/2/II/ioctl_dr_sold.mod
