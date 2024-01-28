cmd_/home/cs695/Lab1/lkm4.mod := printf '%s\n'   lkm4.o | awk '!x[$$0]++ { print("/home/cs695/Lab1/"$$0) }' > /home/cs695/Lab1/lkm4.mod
