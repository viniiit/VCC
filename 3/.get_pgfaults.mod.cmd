cmd_/home/cs695/3/get_pgfaults.mod := printf '%s\n'   get_pgfaults.o | awk '!x[$$0]++ { print("/home/cs695/3/"$$0) }' > /home/cs695/3/get_pgfaults.mod
