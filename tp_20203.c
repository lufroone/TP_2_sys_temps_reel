#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>

#define delai 399900


void do_work(unsigned int dure){
    unsigned long int nbr_boucle = dure * delai;

    for(int i=0;i<nbr_boucle;i++){
        asm volatile("nop");
    }
}

void exec_signal(int signum){
    printf("recu\n");
    do_work(5000);
}

int main(){    
    signal(SIGUSR1, exec_signal);
    printf("le PID est %d\n", getpid());
    while(1){

        printf("en attente\n");
        do_work(1000);
    }
    return 0;
}
