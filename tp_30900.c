#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define delai 39999

void do_work(int iterations) {
    for (int i = 0; i < iterations; i++) {
        asm volatile ("nop");
    }
}

void timer_function(int signum) {
    printf("1s\n");
}

void t2(int temps_exec) {
    do_work(delai * temps_exec);  // Occupera le CPU pendant 0.333s
    printf("t2 terminé\n");
}

void t3(int temps_exec) {
    do_work(delai * temps_exec);  // Occupera le CPU pendant 1s
    printf("t3 terminé\n");
}

void t4(int temps_exec) {
    do_work(delai * temps_exec);  // Occupera le CPU pendant 2s
    printf("t4 terminé\n");
}

int main() {
    struct sigaction sa;
    struct itimerval timer;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_function;
    sigaction(SIGALRM, &sa, NULL);
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    printf("ca part\n");
    while (1) {
        t2(333);
        t3(666);
        pause();
        t3(333);
        t4(666);
        pause();
        t4(1000);
        pause();
        t4(333);
        t2(333);
        t3(333);
        pause();
        t3(666);
        t2(333);
        pause();
        t4(1000);
        pause();
        t4(1000);
        pause();
        t2(333);
        t3(666);
        pause();
        t3(333);
        t2(33);
        t4(333);
        pause();
        t4(1000);
        pause();
        t4(666);
        t2(333);
        pause();
        t3(1000);
        pause();
    }
    return 0;
}