cmd_/home/cs695/3/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/3/modules.order | scripts/mod/modpost -m     -o /home/cs695/3/Module.symvers -e -i Module.symvers -T - 
