cmd_/home/cs695/Lab1/lkm3.mod := printf '%s\n'   lkm3.o | awk '!x[$$0]++ { print("/home/cs695/Lab1/"$$0) }' > /home/cs695/Lab1/lkm3.mod
