cmd_/home/cs695/Lab1/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/Lab1/modules.order | scripts/mod/modpost -m     -o /home/cs695/Lab1/Module.symvers -e -i Module.symvers -T - 
