#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{   
    int *p=(int *)malloc(204800000*sizeof(int));
    printf("Hello world!!!!! %d\n",getpid());
    printf("pointer : %lx\n",p);

    fflush(stdout);
    while(1){
    
    }
    return 0;
}