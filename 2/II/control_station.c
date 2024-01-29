#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>



void sigchld_handler(int sig) {
    int status;
    pid_t sold_pid = wait(&status);
    if (sold_pid == -1) {
        perror("wait");
        exit(1);
    }
    printf("Soldier's process %u terminated\n", sold_pid);
}


int main() {

    struct sigaction sact;
    sact.sa_handler = sigchld_handler;
    sigemptyset(&sact.sa_mask);
    if (sigaction(SIGCHLD, &sact, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    
    printf("central-station process %u started\n", getpid());

    while (1) {
        sleep(1);
    }

    return 0;

}