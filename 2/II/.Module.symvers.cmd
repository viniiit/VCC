cmd_/home/cs695/2/II/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/2/II/modules.order | scripts/mod/modpost -m     -o /home/cs695/2/II/Module.symvers -e -i Module.symvers -T - 
