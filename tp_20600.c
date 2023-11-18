#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define delai 399900


void do_work(){
    unsigned long int nbr_boucle = delai;

    for(int i=0;i<nbr_boucle;i++){
        asm volatile("nop");
    }
}

void alarm_handler(int signum) {
    do_work();
    printf("cest lalarme\n");
    alarm(1);
}

int main() {
    printf("PID du programme : %d\n", getpid());

    signal(SIGALRM, alarm_handler);

    //alarm(1);
    while (1) {
        pause();
    }
    return 0;
}



/*
Pour executer le programme:
gcc -o tp_20600 tp_20600.c
./tp_20600

*/