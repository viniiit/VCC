cmd_/home/cs695/3/hello_procfs.mod := printf '%s\n'   hello_procfs.o | awk '!x[$$0]++ { print("/home/cs695/3/"$$0) }' > /home/cs695/3/hello_procfs.mod
