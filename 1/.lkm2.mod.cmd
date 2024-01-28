cmd_/home/cs695/Lab1/lkm2.mod := printf '%s\n'   lkm2.o | awk '!x[$$0]++ { print("/home/cs695/Lab1/"$$0) }' > /home/cs695/Lab1/lkm2.mod
